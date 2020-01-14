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

  int y;

  //Tell the HMC5883L where to begin reading data
  Wire.beginTransmission(hmc_address);
  Wire.write(0x03); //select register 3, X MSB register
  error = Wire.endTransmission();

  if (error != 0){
    Serial.println("Tranmission error, no response");
    //TODO: Flash LED to indicate error
  }
  
 

  y=readYAxis(hmc_address)
  Serial.print("  y: ");
  Serial.print(y/700.0*90);

  }
  else
  {
    Serial.println("data not ready");
  }
  delay(250);
}

int readYAxis(int address)
{
  int x;

  //Read data from x axis only
  Wire.requestFrom(hmc_address, 4);
  if(6<=Wire.available()){
    Wire.read();            //X Axis byte
    Wire.read();            //X Axis byte
    x = Wire.read()<<8;     //1st Y Byte
    x |= Wire.read();       //2nd Y Byte

  //calibrate y - crude
  //TODO: Auto calibrate by finding range limits and record in EEprom
  if (y > 65000){
    y = y - 65300;
  }
  else
  {
    y=y+253;
  }

  return y;
}

void selectContinuosMode(int address)
{
    Wire.beginTransmission(address);
    Wire.write(0x02); //select mode register
    Wire.write(0x00); //continuous measurement mode
    Wire.endTransmission();
}