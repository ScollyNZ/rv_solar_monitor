#include <Arduino.h>
#include <Wire.h>

#define hmc_address 0x1E //The address shown by the scanner

void selectContinuosMode(int);

void setup(){
  Serial.begin(115200);
  Wire.begin(PIN_WIRE_SDA, PIN_WIRE_SCL);
  
  selectContinuosMode(hmc_address);  //Keep the chip running and reading, instead of sleeping
}

void loop(){
  byte error;


  int x,y,z; //a variable for each axis

  //Tell the HMC5883L where to begin reading data
  Wire.beginTransmission(hmc_address);
  Wire.write(0x03); //select register 3, X MSB register
  error = Wire.endTransmission();

  if (error != 0){
    Serial.println("Tranmission error, no response");
  }
  
 
 //Read data from each axis, 2 registers per axis
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
  else
  {
    Serial.println("data not ready");
  }
  
  delay(250);
}

void selectContinuosMode(int address)
{
    Wire.beginTransmission(address);
    Wire.write(0x02); //select mode register
    Wire.write(0x00); //continuous measurement mode
    Wire.endTransmission();
}