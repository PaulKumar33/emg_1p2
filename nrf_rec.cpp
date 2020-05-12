#include <SPI.h>  
#include "RF24.h" 

RF24 myRadio (7, 8); 
struct package
{
  int val = 0.0;
};

struct instruction{
  char instr;
  float collectionTime = 10;
};

//byte addresses[] = {0xF0, 0xE0}; 
byte addresses[][6] = {"F", "E"};

typedef struct package Package;
typedef struct instruction Instruction;
Package data;
Instruction inst;

void setup() 
{
  Serial.begin(115200);
  delay(1000);

  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_2MBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.openWritingPipe(addresses[1]);
  myRadio.startListening();
  Serial.println("Starting up....");
}


void loop()  
{
    //int val;
    //waits for instruction, then communicates data back

    /*
     * the device should be waiting for a message to start collecting
     * once a command is received from the master - start collecting. 
     * should also recieve the time period for collection
     */
    while (myRadio.available())
    {
      myRadio.read(&inst, sizeof(inst));
      if(inst.instr == 'b'){
        Serial.println("received a begin adc command");

        //send the ack back
        myRadio.stopListening();

        //delay to let the reciever start listening
        delay(100);
        inst.instr = 'a';
        Serial.println(inst.instr);
        myRadio.write(&inst, sizeof(inst));
        Serial.println("Sent the acknowledge");
      }
      /*myRadio.read( &inst, sizeof(inst) );
      if(inst == 'b'){
        //send out the ack back to the master
        myRadio.stopListening();
        char message[] = "ack";
        myRadio.write(&message, sizeof(message));

        //ack sent, now wait for recording period time
        myRadio.startListening();
        int timewait = 10;
        unsigned long StartWaitTime = millis();
        while(millis() - StartWaitTime < timewait*1000){
                          
        }
      }
    }
    Serial.println(data.val);*/
    //myRadio.read( &data, sizeof(data) );
    //Serial.println(data.val);
    }
}