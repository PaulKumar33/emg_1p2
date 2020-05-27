#include <SPI.h>  
#include "RF24.h" 

RF24 myRadio (7, 8); 
struct package
{
  //int id=1;
  int val = 0;
  //char  text[100] = "Text to be transmitted";
};

struct instruction{
  char instr;
  char* ack;
};

struct rfconfig{
  int collectionTime = 10;
};

//byte addresses[] = {0xF0, 0xE0}; 
byte addresses[][6] = {"F", "E"};

typedef struct package Package;
typedef struct instruction Instruction;
typedef struct rfconfig Config;
Config rfConfig;
Package data;
Instruction inst;

/*
 * set up flag globals
 */

bool ACK_RECEIVED;
bool TIME_RECEIVED;

int ackData[2] = {1,1};

void setup() 
{
  Serial.begin(115200);
  //delay(1000);

  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_2MBPS ) ; 
  myRadio.enableAckPayload();
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.openWritingPipe(addresses[1]);
  myRadio.startListening();
  //Serial.println("Starting up....");

  myRadio.writeAckPayload(1, &ackData, sizeof(ackData));
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
        myRadio.writeAckPayload(1, &ackData, sizeof(ackData));

        //send the ack back
        myRadio.stopListening();

        /*//delay to let the reciever start listening
        delay(100);
        inst.instr = 'a';
        myRadio.write(&inst, sizeof(inst));

        //now wait for the collection time
        myRadio.startListening();
        int waitTime = 5;
        delay(100);        
        myRadio.read(&rfConfig, sizeof(rfConfig));
        if(rfConfig.collectionTime != NULL){
          TIME_RECEIVED = true;
        }

        //deal with response
        if(!TIME_RECEIVED){
          //Serial.println("Defaulting to standard time period of 10s");
          rfConfig.collectionTime = 10;
        }

        //begin the collection period*/
        myRadio.stopListening();
        unsigned long collectionStart = millis();
      
         while(millis() - collectionStart < 10000){
          //do collections
          data.val = analogRead(0);
          myRadio.write(&data, sizeof(data));          
         }
      }
   }
}