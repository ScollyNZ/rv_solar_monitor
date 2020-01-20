#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define hmc_address 0x1E //The address shown by the scanner

void selectContinuosMode(int);
int readYAxis(int);
void writeToThingSpeak(int);

void setup(){
  Serial.begin(115200);
  Wire.begin(PIN_WIRE_SDA, PIN_WIRE_SCL);
  
  selectContinuosMode(hmc_address);  //Keep the chip running and reading, instead of sleeping
}

void loop(){
  int y;
  y=readYAxis(hmc_address);
  if (y != -1){
    Serial.print("  y: ");
    y = ((y/340.0*60.0)+30.0);
    Serial.print(y);

    writeToThingSpeak(y);
  }
 

  Serial.print("sleeping");
  delay(300000);
}

void writeToThingSpeak(int value)
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
        "https://api.thingspeak.com/update?api_key=OI12E35F4LBS3PDO&field1="+String(value),
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

int readYAxis(int address)
{
  Serial.print("beginning read cycle");
  byte error;

  int x,y=0,z;
 
  //Tell the HMC5883L where to begin reading data
  Wire.beginTransmission(hmc_address);
  Wire.write(0x03); //select register 5, Y MSB register
  error = Wire.endTransmission();

  if (error != 0){
    Serial.println("Tranmission error, no response");
    //TODO: Flash LED to indicate error
    return -1;
  }
  Serial.println("reading");

  //Read data from x axis only
  Wire.requestFrom(hmc_address, 6);
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb

    Serial.print("x: ");
    Serial.print(x);
    Serial.print("  y: ");
    Serial.print(y);
    Serial.print("  z: ");
    Serial.println(z);
  }

  //calibrate y - crude
  //TODO: Auto calibrate by finding range limits and record in EEprom
    y = y - 65105;

  return y;
}

void selectContinuosMode(int address)
{
    Wire.beginTransmission(address);
    Wire.write(0x02); //select mode register
    Wire.write(0x00); //continuous measurement mode
    Wire.endTransmission();
}