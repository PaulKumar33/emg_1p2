#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (7, 8);
//byte addresses[] = {0xF0, 0xE0};
byte addresses[][6] = {"F", "E"};
struct package
{
  //int id=1;
  int val = 0;
  //char  text[100] = "Text to be transmitted";
};

struct instruction{
  char instr;
  char* ack;
  float collectionTime = 10;
};


typedef struct package Package;
typedef struct instruction Instruction;
Package data;
Instruction inst;


void setup()
{
  Serial.begin(115200);
  delay(1000);
  //Serial.println("0x%x", addresses[0]);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_2MBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  myRadio.openReadingPipe(1, addresses[1]);
  delay(1000);
}

void loop()
{
  if(Serial.available()>0){
    if(Serial.read() == 'b'){
      Serial.println("beginning Data Collection...");

      //want to send an instr
      inst.instr = 'b';
      myRadio.write(&inst, sizeof(inst));
      myRadio.startListening();
      Serial.println("listening...");

      unsigned long waitTime = millis();
      int timePeriod = 5;
      

      //works as interrupt like flow -> chekc for ack
      myRadio.read(&inst, sizeof(inst));
      Serial.println(inst.instr);
      while(inst.instr != 'a'){
        if(millis() - waitTime > timePeriod*1000){
          break;
        }      
      }
      if(inst.instr == 'a'){
        Serial.println("Recieved ack");
        myRadio.stopListening();

        //sending the collection time period.
        inst.collectionTime = 10;
        myRadio.write(&inst, sizeof(inst)); 
      }

      //now prepare to send the time period
      /*
       * need to rework this. eventually want to recieve a python struct with the time and
       * other collection params
       * 
       * for now it is defaulted to 10
       */
       
       inst.collectionTime = 10;
       myRadio.write(&inst, sizeof(inst));
       
    }
    if(Serial.read() == 'k'){
      Serial.println("Kill command sent");
    }
    //data.val = analogRead(0);  
    //myRadio.write(&data.val, sizeof(data.val)); 
    //Serial.println(data.val);  
  }
  /*data.val = analogRead(0);  
  myRadio.write(&data.val, sizeof(data.val)); 
  Serial.println(data.val); 
  //delay(10000);*/
}