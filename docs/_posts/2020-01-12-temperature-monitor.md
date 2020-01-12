---
layout: post
title:  "IOT Cloche - Temperature Monitor"
tags: "esp general" "IOT" "IOT cloche"
categories: "IOT Cloche"
---

# Cloche Temperature Monitoring

Step one of the IOT connected cloche project is to monitor two temperature probes and dashboard them. The probes will be positioned inside and outside the cloche to record the difference the cloche is making to the temperature the seedlings are exposed to.

## Temperature Probe

The [LM75](http://www.ti.com/lit/ds/symlink/lm75b.pdf) is an I2C enable thermistor that is very simple to interface with the ESP8266 due to I2C being readily available.

The [chip](https://www.aliexpress.com/item/32678279458.html?spm=a2g0s.9042311.0.0.27424c4dVpeVw5) is available on carrier boards but I purchased many surface mount chips and hand soldered them onto [SMD boards](https://www.aliexpress.com/item/32592419821.html?spm=a2g0s.9042311.0.0.27424c4dVpeVw5) because I like to make things hard for myself.

The only issue I had interfacing this chip with the ESP is that is wasn't immediately obvious which pins were the I2C pins on the ESP. [This](https://www.instructables.com/id/ESP8266-I2C-PORT-and-Address-Scanner/) instructable provides code for an I2C scanner. I had written something similar but this example also discovers pin numbers of the I2C bus, which is a bonus.

[This](http://www.esp8266learning.com/esp8266-lm75-temperature-sensor-example.php) blog post shows the most basic set up for temperature monitoring with output via the serial port. Interesting that it is lacking the pull resistors that normally feature on an I2C bus, maybe with only one device present the pull ups aren't needed? something to experiment with later.

After a short hunt I found [i2c_port_address_scanner.ino](https://github.com/jainrk/i2c_port_address_scanner/blob/master/i2c_port_address_scanner/i2c_port_address_scanner.ino) and have comitted it at [utilities/i2c_scanner.cpp](../../src/utilities/i2c_scanner.cpp) with minor modifications so it compiles under PlatformIO.

Unsuprisingly, when I ran it on an ESP straight out of the bag, it didn't tell me much.

```
I2C Scanner to scan for devices on each port pair D0 to D7
Scanning (SDA : SCL) - GPIO16 : GPIO5 - No I2C devices found
Scanning (SDA : SCL) - GPIO16 : GPIO4 - No I2C devices found
Scanning (SDA : SCL) - GPIO16 : GPIO0 - No I2C devices found
Scanning (SDA : SCL) - GPIO16 : GPIO2 - No I2C devices found
Scanning (SDA : SCL) - GPIO16 : GPIO14 - No I2C devices found
Scanning (SDA : SCL) - GPIO16 : GPIO12 - No I2C devices found
Scanning (SDA : SCL) - GPIO16 : GPIO13 - No I2C devices found
Scanning (SDA : SCL) - GPIO5 : GPIO16 - No I2C devices found
Scanning (SDA : SCL) - GPIO5 : GPIO4 - No I2C devices found
Scanning (SDA : SCL) - GPIO5 : GPIO0 - No I2C devices found
Scanning (SDA : SCL) - GPIO5 : GPIO2 - No I2C devices found
Scanning (SDA : SCL) - GPIO5 : GPIO14 - No I2C devices found
Scanning (SDA : SCL) - GPIO5 : GPIO12 - No I2C devices found
Scanning (SDA : SCL) - GPIO5 : GPIO13 - No I2C devices found
Scanning (SDA : SCL) - GPIO4 : GPIO16 - No I2C devices found
Scanning (SDA : SCL) - GPIO4 : GPIO5 - No I2C devices found
Scanning (SDA : SCL) - GPIO4 : GPIO0 - No I2C devices found
Scanning (SDA : SCL) - GPIO4 : GPIO2 - No I2C devices found
Scanning (SDA : SCL) - GPIO4 : GPIO14 - No I2C devices found
Scanning (SDA : SCL) - GPIO4 : GPIO12 - No I2C devices found
Scanning (SDA : SCL) - GPIO4 : GPIO13 - No I2C devices found
Scanning (SDA : SCL) - GPIO0 : GPIO16 - No I2C devices found
Scanning (SDA : SCL) - GPIO0 : GPIO5 - No I2C devices found
Scanning (SDA : SCL) - GPIO0 : GPIO4 - No I2C devices found
Scanning (SDA : SCL) - GPIO0 : GPIO2 - No I2C devices found
Scanning (SDA : SCL) - GPIO0 : GPIO14 - No I2C devices found
Scanning (SDA : SCL) - GPIO0 : GPIO12 - No I2C devices found
Scanning (SDA : SCL) - GPIO0 : GPIO13 - No I2C devices found
Scanning (SDA : SCL) - GPIO2 : GPIO16 - No I2C devices found
Scanning (SDA : SCL) - GPIO2 : GPIO5 - No I2C devices found
Scanning (SDA : SCL) - GPIO2 : GPIO4 - No I2C devices found
Scanning (SDA : SCL) - GPIO2 : GPIO0 - No I2C devices found
Scanning (SDA : SCL) - GPIO2 : GPIO14 - No I2C devices found
Scanning (SDA : SCL) - GPIO2 : GPIO12 - No I2C devices found
Scanning (SDA : SCL) - GPIO2 : GPIO13 - No I2C devices found
Scanning (SDA : SCL) - GPIO14 : GPIO16 - No I2C devices found
Scanning (SDA : SCL) - GPIO14 : GPIO5 - No I2C devices found
Scanning (SDA : SCL) - GPIO14 : GPIO4 - No I2C devices found
Scanning (SDA : SCL) - GPIO14 : GPIO0 - No I2C devices found
Scanning (SDA : SCL) - GPIO14 : GPIO2 - No I2C devices found
Scanning (SDA : SCL) - GPIO14 : GPIO12 - No I2C devices found
Scanning (SDA : SCL) - GPIO14 : GPIO13 - No I2C devices found
Scanning (SDA : SCL) - GPIO12 : GPIO16 - No I2C devices found
Scanning (SDA : SCL) - GPIO12 : GPIO5 - No I2C devices found
Scanning (SDA : SCL) - GPIO12 : GPIO4 - No I2C devices found
Scanning (SDA : SCL) - GPIO12 : GPIO0 - No I2C devices found
Scanning (SDA : SCL) - GPIO12 : GPIO2 - No I2C devices found
Scanning (SDA : SCL) - GPIO12 : GPIO14 - No I2C devices found
Scanning (SDA : SCL) - GPIO12 : GPIO13 - No I2C devices found
Scanning (SDA : SCL) - GPIO13 : GPIO16 - No I2C devices found
Scanning (SDA : SCL) - GPIO13 : GPIO5 - No I2C devices found
Scanning (SDA : SCL) - GPIO13 : GPIO4 - No I2C devices found
Scanning (SDA : SCL) - GPIO13 : GPIO0 - No I2C devices found
Scanning (SDA : SCL) - GPIO13 : GPIO2 - No I2C devices found
Scanning (SDA : SCL) - GPIO13 : GPIO14 - No I2C devices found
Scanning (SDA : SCL) - GPIO13 : GPIO12 - No I2C devices found
```

However, when the same board was plugged into an old project that had multiple LM75s still wired to it, the results were better

```
I2C Scanner to scan for devices on each port pair D0 to D7
Scanning (SDA : SCL) - GPIO16 : GPIO5 - No I2C devices found

<<< snip >>>

Scanning (SDA : SCL) - GPIO5 : GPIO16 - No I2C devices found
Scanning (SDA : SCL) - GPIO5 : GPIO4 - I2C device found at address 0x49  !
I2C device found at address 0x4A  !
I2C device found at address 0x4B  !
I2C device found at address 0x4C  !
I2C device found at address 0x4D  !
I2C device found at address 0x4E  !
I2C device found at address 0x4F  !
**********************************

<<< snip >>>
```
So. GPIO05 and GPIO04 are the pins I'm looking for. I could have achieved that by reviewing the code from the previous project or examining the PCB but now I know the I2C scanner code works.

## Next Steps

The next steps are more mechanical in nature. My previous temperature monitoring project was fairly unsuccessful because, I think, the LM75s were not thermally isolated from the wood they are sitting in. Additionally, with this project, they need to be protected from water. I also want them to be measuring the air temperature, rather than the sun's radiance so some shading will be important.

