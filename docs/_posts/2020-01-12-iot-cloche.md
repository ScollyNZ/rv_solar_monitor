---
layout: post
title:  "IOT Cloche - Project Brief"
tags: "esp general" "IOT" "IOT cloche"
---

# IOT Cloche

In the last few months I've wanted to get back into gardening, especially for fresh greens and herbs. The weather in Christchurch is very variable and the temperature can change from 25+℃ to below 10℃ in teh space of 24 hours. I'm also really slack at regularly watering the seedlings so the death rate is higher than it should be.

The IOT Cloche project aims to solve a few of these issues by bringing some automation to bear.

The cloche itself is a wooden boxed with a double hinged lid. The lid consists of a wooden frame, hinged from the bottom, connected to a [temperature activated actuator arm](https://www.aliexpress.com/item/32888524914.html?spm=a2g0s.9042311.0.0.27424c4dkhZwx0). This opens the cloche when the temperatures rises.

Attached to this frame are two pieces of clear acrylic, hinged from the top, to allow easy access to the seedlings.

A recent addition is a row of mister heads at the top of the cloche for water. These currently run for 5 minutes, twice daily.

The IOT part of this projects is to first monitor, then control the irrigation. Using an ESP8266 and some Azure or similar services I plan to

1. Monitor the temperature inside and outside to cloche  
   Monitoring these temperatures will help me determine of the cloche is having a stabilising effect on the seedlings temperature. I'm also interested in how much the cloche extends the growing season by keeping the temperature high enough over night to allow greenery to develop properly. I have some LM75 I2C thermistors from a previous project that should make this fairly straight forward.

1. Monitor the open state of the cover  
The cover is entirely mechanically operated, requiring no power or logic input so I'll add a micro-switch in a position that switches on when the lid opens sufficiently. If the cloche works as I hope it does the correlation between internal temp, external temp and the lid opening should be observable by the internal temperature stabilising as the lid opens to let more or less external air movement.

1. Monitor soil moisture level  
The cloche is sized to hold 4 seedling trays. These trays can be monitored individually to see how quickly water to removed from teh seedlings or just general evaporation.

1. Control water flow  
Once soil moisture levels are known the irrigation can be optimised to keep the soil moisture level consistent.

Once all this data has been collected some experiments can be run to optimise the conditions in the cloche. The big unknown at present is how to monitor plant growth. There are a couple of options, but the most interesting is to use some sort of machine vision to measure the amount of green. This could work with a 'standardised' planting method, a measure amount of seed or something similar.

There are several fairly cheap [camera options for the esp32](https://www.aliexpress.com/item/33011096369.html?spm=a2g0o.productlist.0.0.22b05cdc3mHBRh&s=p&algo_pvid=e983737a-0756-475d-ab02-59a6f66dda37&algo_expid=e983737a-0756-475d-ab02-59a6f66dda37-2&btsid=95271bd8-903e-41a9-9398-9f8d2309a2be&ws_ab_test=searchweb0_0,searchweb201602_10,searchweb201603_55). It would be an interesting exercise to count the green pixels with onboard processing but uploading the image for processing may be a simpler solution, especially as once a day would be a more than adequate update frequency.

...end...

