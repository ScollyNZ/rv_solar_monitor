#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "ADS1115.h"

ADS1115 adcBatteryI(ADS1115_ADDRESS_ADDR_SDA); 
ADS1115 adcInverterI(ADS1115_ADDRESS_ADDR_GND); 
ADS1115 adcBatteryV(ADS1115_ADDRESS_ADDR_VDD); 

void writeToThingSpeak(float, float);

void setup() {                
    Wire.begin(PIN_WIRE_SDA, PIN_WIRE_SCL);  // join I2C bus
    Serial.begin(115200); // initialize serial communication 
    Serial.println("Initializing I2C devices..."); 
    adcBatteryI.initialize(); 
    adcInverterI.initialize(); 
    adcBatteryV.initialize(); 
    
    Serial.println("Testing device connections...");
    Serial.println(adcBatteryI.testConnection() ? "BatteryI connection successful" : "BatteryI connection failed");
    Serial.println(adcInverterI.testConnection() ? "InverterI connection successful" : "InverterI connection failed");
    Serial.println(adcBatteryV.testConnection() ? "BatteryV connection successful" : "BatteryV connection failed");
      
    // We're going to do continuous sampling
    adcBatteryI.setMode(ADS1115_MODE_CONTINUOUS);
    adcInverterI.setMode(ADS1115_MODE_CONTINUOUS);
    adcBatteryV.setMode(ADS1115_MODE_CONTINUOUS);
}

void loop() {
    if (false)
    {
    // Set the gain (PGA) +/- 256mv
    adcBatteryI.setGain(ADS1115_PGA_0P256);
    adcInverterI.setGain(ADS1115_PGA_0P256);
    adcBatteryV.setGain(ADS1115_PGA_4P096);

    float mvPerAmp50Amp = 50.0/75.0;   //50A shunt, 75mv full range
    float mvPerAmp150Amp = 150.0/75.0;   //150A shunt, 75mv full range
    float mvPerVolt = 24.0*(1/7)*1000;        //Voltage divider 1k-6k=7k
    float readingBatteryAmps=adcBatteryI.getConversionP0N1()*adcBatteryI.getMvPerCount()*mvPerAmp50Amp;
    float readingInverterAmps=adcInverterI.getConversionP2N3()*adcInverterI.getMvPerCount()*mvPerAmp150Amp;


    int loopCount = 1;
    float averageBatteryAmps = readingBatteryAmps;
    float averageInverterAmps = readingInverterAmps;

    while (loopCount < (5*60))  //Take an average for 5 minutes
    {
        float readingBatteryAmps=adcBatteryI.getConversionP0N1()*adcBatteryI.getMvPerCount()*mvPerAmp50Amp;
        float readingInverterAmps=adcInverterI.getConversionP2N3()*adcInverterI.getMvPerCount()*mvPerAmp150Amp;

        //Approximate rolling average
        averageBatteryAmps -= averageBatteryAmps / loopCount;
        averageBatteryAmps += readingBatteryAmps / loopCount;

        averageInverterAmps -= averageInverterAmps / loopCount;
        averageInverterAmps += readingInverterAmps / loopCount;

        Serial.print("Battery Current: ");
        Serial.print(readingBatteryAmps);
        Serial.print(" Average:");
        Serial.print(averageBatteryAmps);
        Serial.print(" Inverter Current: ");
        Serial.print(readingInverterAmps);
        Serial.print(" Average:");
        Serial.print(averageInverterAmps);
        Serial.print(" Sample #:");
        Serial.println(loopCount);

        delay(1000);
        loopCount++;
    }

    Serial.println("***************** Upload averages ********************");
    writeToThingSpeak(averageBatteryAmps, averageInverterAmps);
    }

}

void writeToThingSpeak(float field1, float field2)
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
        "https://api.thingspeak.com/update?api_key=GQVDV0IP91VYE9W6&field1="+String(field1)+"&field2="+String(field2),
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
