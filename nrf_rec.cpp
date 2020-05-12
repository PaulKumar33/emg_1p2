#include <SPI.h>  
#include "RF24.h" 

RF24 myRadio (7, 8); 
struct package
{
  int val = 0.0;
};

struct instruction{
  char instr;
  float collectionTime = null;
};

//byte addresses[] = {0xF0, 0xE0}; 
byte addresses[][6] = {"F", "E"};

typedef struct package Package;
typedef struct instruction Instruction;
Package data;
Instruction inst;

/*
 * set up flag globals
 */

bool ACK_RECEIVED;
bool TIME_RECEIVED;

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

  //initialize flags
  ACK_RECEIVED = false;
  TIME_RECEIVED = false;
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

        //now wait for the collection time
        myRadio.startListening();
        int waitTime = 5; //wait time for collection time
        unsigned long startWait = millis();
        
        myRadio.read(&inst, sizeof(inst));
        while(millis() - waitTime < waitTIme*1000){
          //hold and then send timeout signal
          TIME_RECEIVED = false;
          if(inst.collectionTime != null){
            TIME_RECEIEVED = true;
            break;
          }
          myRadio.read(&inst, sizeof(inst));          
        }

        //deal with response
        if(!TIME_RECEIVED){
          Serial.println("Defaulting to standard time period of 10s");
          inst.collectionTime = 10;
        }

        //else condition not needed. inst.collectionPeriod should be set

        //begin the collection period
        unsigned long collectionStart = millis()
        /*
         * configure the ADC here
         */

         while(millis() - collectionStart < inst.collectionTime){
          //do collections
         }
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