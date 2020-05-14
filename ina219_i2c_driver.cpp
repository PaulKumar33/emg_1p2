
#include "Wire.h"
uint8_t i2c_address = 0x40;

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
      //now read back the register contents
      Wire.requestFrom(i2c_address, 2);
      byte a = Wire.read();
      byte b = Wire.read();

      uint16_t read_words = ((a << 8) | b);
      Serial.println(read_words, HEX);
      if(read_words - 0x399F == 0){
        Serial.println("POR Success");
      }
      else{
        Serial.print("Config reg read back ");
        Serial.print(read_words, HEX);
        Serial.print(" When 0x399F expected");
        return;
        } 
    }
    else{
      Serial.println("Could not send POR....");
      return;
      }
    delay(1000);
  }
  
}