---
layout: post
title:  "IOT Cloche - Soil Moisture Monitor"
categories: "IOT Cloche"
---

# Soil Moisture - Step 1

I recently upgraded the cloche with a set of mister heads. There are now 6 mister heads along the back wall of the cloche keeping the seedlings moist. This has significantly reduced the mortality rate but I'm fairly sure I'm over watering.

Step one of soil moisture is monitoring it. Steps two and three are to control it and then to measure water usage.

Control will consist of a water solenoid to turn on the mister heads under two scenarios  
1. When the soil is too dy, probably at a given time of the day, rather than at random
1. When the temperature in the cloche rises above preset value.

## Moisture Monitoring

A quick visit to Ali Express identified a couple of required components. The first is [the sensor itself](https://www.aliexpress.com/item/32951556560.html?spm=a2g0s.9042311.0.0.27424c4dlyeWmX) which is a fairly simple probe and op-amp arrangement. It also has a digital output that can be triggered by a pre-set value.

The second issue is that the ESP8266 Mini D1, that I'm using, has only 1 analog input port. This is also an issue on the solar monitor project. Turns out this is a solved problem, the solution being an I2C enabled ADC known as the ADS1115. It, also, is [available on Ali Express](https://www.aliexpress.com/item/1656318134.html?spm=a2g0s.9042311.0.0.7f994c4dnzknWT).

![Sensor, ADC and ESP](images/moisture_components.jpg)

[This](https://github.com/addicore/ADS1115/blob/master/ADS1115.cpp) looks like a well documented class for interfacing with the ADC but first I need to investigate the soil moisture probe as it comes with no documentation at all.

## Moisture Sensor

A quick test rig using a prototype board and an ESP as a 3v3 power supply showed all was working as expected. The multi-meter showed the voltage difference between the 3v3 rail and the A0 pin as follows

|Contact|Voltage|
|---|---|
|Nothing|0.0v|
|Dry Fingers|0.6v|
|Wet Surface|3.2v|

![Moisture Test Rig](images/soil_moisture_1.jpg)

The ADS1115 has a selectable range option, according to the [datasheet](https://www.ti.com/lit/ds/symlink/ads1115.pdf), from 256mv to 6.1v in 6 steps.

The datasheet states

```
Analog input voltages must never exceed the analog input voltage limits given in the Absolute Maximum Ratings.If a VDD supply voltage greater than 4V is used,the ±6.144V full-scale range allows input voltages to extend up to the supply.Although in this case(or whenever the supply voltage is less than the full-scale range;for example,VDD= 3.3 V and full-scale range = ±4.096V), a full-scale ADC output code cannot be obtained.For example,with VDD= 3.3 V and FSR= ±4.096V, only signals up to VIN= ±3.3 V can be measured.The code range that represents voltages|VIN| > 3.3 V is not used in this case.
```

Since I'm using 3v3 the ±4.096V setting is the one to go for, giving access to maximum range. Although I do have 5V on hand so not really sure why I'm using 3v3.

|Contact|Voltage|
|---|---|
|Nothing|0.0v|
|Dry Fingers|0.8v|
|Wet Surface|4.2v|

In which case, ±6.144V looks like the better option.

## I2C Hook Up

Out with the prototype boards again to 5v power, gnd, D5 to SCL, D4 to SDA and running [I2CScanner](../../src/utilities/i2c_scanner.cpp). The address on the ADS1115 is a little different from what I've experienced before. The chips I've used in the past have had multiple address pins to indicate the address using binary (+5v/Gnd) connections to indicate high and low. The LM75 is a good example of this. The ADS1115 uses a single address pin and determines address by which ever other pin it is connected to, as per the table below

Table 4. ADDR Pin Connection and Corresponding Slave Address
|ADDR PIN CONNECTION | SLAVE ADDRESS| HEX Address | Decimal|
|---|---|---|---|
|GND|1001000|0x48|74|
|VDD|1001001|0x49|75|
|SDA|1001010|0x4A|76|
|SCL|1001011|0x4B|77|

I'm not sure why, but this is the same address space the LM75 uses meaning a maximum of 4 devices before needing an I2C multiplexer. Presumably its possible to run two separate buses by changing the pin configuration on the Wire.Begin call, something to try at some point.

I2CScanner strikes again

```
I2C Scanner to scan for devices on each port pair D0 to D7
Scanning (SDA : SCL) - GPIO16 : GPIO5 - No I2C devices found
<<snip>>
Scanning (SDA : SCL) - GPIO16 : GPIO13 - No I2C devices found
Scanning (SDA : SCL) - GPIO5 : GPIO16 - No I2C devices found
Scanning (SDA : SCL) - GPIO5 : GPIO4 - I2C device found at address 0x48  !
**********************************
<<snip>>
```

With ADDR connected to GND the device appears at 0x48.

## Software

It turns out a Mr J Rowberg has built an [I2C software library](https://github.com/jrowberg/i2cdevlib) that is available in the PlatformIO plug in for VS Code. After a quick install of the ADS1115 library the code started taking shape.

The sample file that came with the library required a couple of changes for my purposes. Firstly, I use 115,200bps for serial, not 19,200, easy fix. Also, my needs require the Pga to be set to 6.144, since I using 5v. I also removed the the code that read from the second sensor, one thing at a time. VS Code intellisense didn't work until I restarted after installing the library.

The software failed to detect the ADS1115 on the bus during the first run. A second run, after specifying the I2C pins in the wire.begin call still failed. I2CScanner can still see the device at 0x49, which is ADS1115_ADDRESS_ADDR_VDD not ADS1115_DEFAULT_ADDRESS which is ADS1115_ADDRESS_ADDR_GND. Quick code change and try again...

Communication with the chips seems to have been established, software reports success and values are output, but touching the moisture probe doesn't have any affect. Connection A0 directly to GND changes the reading to 4.5mv and connection to VDD gives a reading of 4500mv so the ADC is working.

After replugging it looks to be working except the readings are the reverse of what I'm looking for (4.5V is dry rather than wet) so added

```
adc0.setComparatorPolarity(ADS1115_COMP_POL_ACTIVE_HIGH);
```

Which had no affect at all...


...end...