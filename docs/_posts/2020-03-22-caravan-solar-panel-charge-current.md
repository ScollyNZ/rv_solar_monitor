---
layout: post
title:  "IOT Caravan - Solar Current Monitoring"
categories: "Solar Monitor"
---

# Solar Current Monitor

The solar panels are mounted, the batteries are mounted, the inverter and fridge are happy. All I need now is some monitoring so I can see what is going on.

The wiring panel needs a tidy up but the solar charge controller, circuit breaks, inverter and shunt are wired and don't produce any smoke. I was going to use the solar panel circuit breaker as the shunt, rather than introducing another point of resistance in the circuit, but if the breaker blows, or is turned off, the voltage difference between the two sides would be the voltage of the solar panels. This is usually up in the ~40V range, which would probably have a negative affect on the ADC so the shunt was put in place.

The mission today is to work out how to configure the ADC for differential measuring. I need to use differential because I don't have a common -ve for every measure point due to the charge controller doing its magic. Differential requires two inputs into the ADC, instead of just one, and measure the difference between the two inputs rather than the difference between one input and common ground.

The shunt is 75mv / 50A meaning each mv indicates 0.6667 amps. Its a sunny day today and the batteries are at 25.5V. The charge controller is configured to take them up to 27.5 so its pushing all it can into the batteries. My multimeter reads 7.5mv across the shunt. 7.5 * 0.6667 = 5 Amps. 5 Amps at 25.5 volts = 127.5 watts which is not too bad given the panels are sitting flat against the roof of the caravan.

## The ADC

As mentioned previously I'm using the ADS1115. There are two configuration items I need to set before taking any readings. The programmable gain amplifier and the input pin mode. As mentioned above, I'll need it to be differential. The sampling rate can also be wound down to 8/sec.

The critical lines of code are

```
    adc0.setGain(ADS1115_PGA_0P256);    //Set ADC to most sensitive
    ...
    ...
    int sensorOneCounts=adc0.getConversionP0N1();   // do differential read
    
```

This setup works fine for about 30 seconds, then it starts missing readings. Turns out the ADC is becoming _really_ hot, but surviving.

A quick poke around the Internet turn up [this](https://electronics.stackexchange.com/questions/424464/shunt-resistor-with-ads1115) post on stack exchange, which documents pretty much exactly what I'm wanting to do. One the the replies points out that he can't see a reason why anyone would measure the current on the solar panel side. I see his point, so I'll move my shunt to the battery side of the controller.

After this change all is good. The ADC is not hot to the touch and the readings are stable, I obviously have more to learn...

All was good, until I realised I'd left teh inverter that was powering the laptop off, turned it back on and the chip became hot again - power supply issue.

The interesting point from the above post is that all shunts have a common ground meaning differential isn't really needed. I need to do a rethink but running of an isolated power supply (my laptop battery) all seems good.

```
.93Mv, 5.95 Amps
8.71Mv, 5.81 Amps
9.04Mv, 6.03 Amps
8.71Mv, 5.81 Amps
8.65Mv, 5.77 Amps
8.77Mv, 5.85 Amps
8.67Mv, 5.78 Amps
8.91Mv, 5.94 Amps
8.63Mv, 5.76 Amps
8.99Mv, 6.00 Amps
8.63Mv, 5.76 Amps
8.86Mv, 5.91 Amps
8.79Mv, 5.86 Amps
8.59Mv, 5.73 Amps
8.77Mv, 5.85 Amps
8.73Mv, 5.82 Amps
9.02Mv, 6.02 Amps
8.63Mv, 5.75 Amps
```

I need to implement some voltage measurement so I can calculate watts, but thats another step.

...end...