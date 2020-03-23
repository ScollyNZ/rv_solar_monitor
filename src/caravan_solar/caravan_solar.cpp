#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "ADS1115.h"

ADS1115 adc0(ADS1115_ADDRESS_ADDR_VDD); 

void writeToThingSpeak(float value);

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
    // Set the gain (PGA) +/- 256mv
    adc0.setGain(ADS1115_PGA_0P256);
    float mvPerAmp = 50.0/75.0;   //50A shunt, 75mv full range
    float readingAmps=adc0.getConversionP0N1()*adc0.getMvPerCount()*mvPerAmp;

    int loopCount = 1;
    float averageAmps = readingAmps;

    while (loopCount < (5*60))  //Take an average for 5 minutes
    {
        readingAmps=adc0.getConversionP0N1()*adc0.getMvPerCount()*mvPerAmp;

        //Approximate rolling average
        averageAmps -= averageAmps / loopCount;
        averageAmps += readingAmps / loopCount;

        Serial.print("Current: ");
        Serial.print(readingAmps);
        Serial.print(" Average:");
        Serial.print(averageAmps);
        Serial.print(" Sample #:");
        Serial.println(loopCount);

        delay(1000);
        loopCount++;
    }

    Serial.println("***************** Upload 5 min average ********************");
    writeToThingSpeak(averageAmps);

}

void writeToThingSpeak(float value)
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
