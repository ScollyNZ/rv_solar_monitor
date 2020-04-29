// I2C device class (I2Cdev) demonstration Arduino sketch for ADS1115 class
// Example of reading two differential inputs of the ADS1115 and showing the value in mV
// 06 May 2013 by Frederick Farzanegan (frederick1@farzanegan.org)
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     2013-05-13 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/
#include "ADS1115.h"

ADS1115 adc0(ADS1115_ADDRESS_ADDR_VDD); 

void setup() {                
    Wire.begin(PIN_WIRE_SDA, PIN_WIRE_SCL);  // join I2C bus
    Serial.begin(115200); // initialize serial communication 
    Serial.println("Initializing I2C devices..."); 
    adc0.initialize(); // initialize ADS1115 16 bit A/D chip
    
    Serial.println("Testing device connections...");
    Serial.println(adc0.testConnection() ? "ADS1115 connection successful" : "ADS1115 connection failed");
      
    // To get output from this method, you'll need to turn on the 
    //#define ADS1115_SERIAL_DEBUG // in the ADS1115.h file
    adc0.showConfigRegister();
    
    // We're going to do continuous sampling
    adc0.setMode(ADS1115_MODE_CONTINUOUS);
}

void loop() {
    // voltage divider is 1k/6.7k
    // so 26v should be (26/7.7*1) and (26/7.7*2.7)
    //  3.37 and 22.6 = 26 - math is good
    adc0.setGain(ADS1115_PGA_4P096);
    float readingVolts=adc0.getConversionP0GND()*adc0.getMvPerCount();

    adc0.setGain(ADS1115_PGA_0P256);
    float readingBatteryAmps=adc0.getConversionP1GND()*adc0.getMvPerCount();

    int loopCount = 1;
    float averageVolts = readingVolts;
    float averageBatteryAmps = readingBatteryAmps;

    while (loopCount < (10*60))  //Take an average for 10 minutes
    {
        adc0.setGain(ADS1115_PGA_4P096);
        float readingVolts=adc0.getConversionP0GND()*adc0.getMvPerCount();

        adc0.setGain(ADS1115_PGA_0P256);
        float readingBatteryAmps=adc0.getConversionP1GND()*adc0.getMvPerCount();

        //Approximate rolling average
        averageVolts -= averageVolts / loopCount;
        averageVolts += readingVolts / loopCount;

        //Approximate rolling average
        averageBatteryAmps -= averageBatteryAmps / loopCount;
        averageBatteryAmps += readingBatteryAmps / loopCount;


        Serial.print("Voltage: ");
        Serial.print(readingVolts);
        Serial.print(" Average:");
        Serial.print(averageVolts);
        Serial.print("Battery I: ");
        Serial.print(readingBatteryAmps);
        Serial.print(" Average:");
        Serial.print(averageBatteryAmps);
        Serial.print(" Sample #:");
        Serial.println(loopCount);

        delay(1000);
        loopCount++;
    }

    Serial.println("***************** Upload 10 min average ********************");

    //writeToThingSpeak(averageVolts);

}

/*
void writeToThingSpeak(averageVolts)
{
    //yes, I know my wifi password is in git. 
    //yes, if you know where I live and want to use my WiFi you can
    //yes, if I find out I'll change my password
    WiFi.begin("scoltock", "nowireshere");  
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print("Connecting..");
    }

    Serial.print("WiFi connected");

    HTTPClient http;
    http.begin(
        "https://api.thingspeak.com/update?api_key=GQVDV0IP91VYE9W6&field1="+String(value),
         "F9 C2 65 6C F9 EF 7F 66 8B F7 35 FE 15 EA 82 9F 5F 55 54 3E");

    //http.begin(WiFi.,"api.thingspeak.com",443,"/update?api_key=OI12E35F4LBS3PDO&field1="+String(value),true);

    int httpResponseCode = http.GET();

    String payload = http.getString();
    Serial.println("HTTP Response Code: " + String(httpResponseCode,0));
    Serial.println("HTTP Response Body: " + payload);

    http.end();
    WiFi.disconnect();
    
    Serial.print("WiFi disconnected");

}
*/
