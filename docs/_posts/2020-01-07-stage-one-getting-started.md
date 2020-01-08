---
layout: post
title:  "Environment Set Up"
---
# Environment Set Up

VSCode is gaining ground rapidly as the editor of choice for many different projects. It turns out that a reasonable amount of effort has been put into bring Arduino projects into VSCode which, given the limitations of the Arduino IDE, seems well worth the effort to set up.

## Documentation

The documentation for this project is published using Github Pages and two VSCode extension "Markdown All In One" and "Markdown Preview Enhanced".

## Arduino Set Up for ESP8266

[This](https://cuneyt.aliustaoglu.biz/en/enabling-arduino-intellisense-with-visual-studio-code/) blog post details how one person set up VSCode with Arduino. Essentially we need to 

1. Install the Arduino VSCode Extension
1. Install the "ESP8266FS" companion plug in for uploading to the ESP chips
1. Add the path to an install of the Arduino IDE into the VSCode settings
1. Leave 'select programmer' as is
1. Change 'board type' to 'Generic ESP8266 Module (esp8266)'
1. plug in an esp and select /dev/ttyUSB0 or similar