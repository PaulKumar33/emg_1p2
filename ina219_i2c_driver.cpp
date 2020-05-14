
#include "Wire.h"
byte i2c_address = 0x40;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly
  Wire.beginTransmission(i2c_address);
  if(Wire.endTransmission() == 0){
    Serial.println("ACK received");
    //configure for power on reset
    Wire.beginTransmission(i2c_address);
    Wire.write(0x00);
    Wire.write(0x39);
    Wire.write(0x9F);

    //send byte, see if we recieve an ack
    if(Wire.endTransmission()==0){
      Serial.println("POR");
    }
    delay(1000);
  }
  
}