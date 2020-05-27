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
};

struct rfconfig{
  int collectionTime = 10;
};

//initialize structures
typedef struct package Package;
typedef struct instruction Instruction;
typedef struct rfconfig Config;
Config rfConfig;
Package data;
Instruction inst;

//variables for reading fromPC
boolean readInProgress = false;
boolean newDataFromPC = false;
const char buffSize =40;
char inputBuffer[buffSize];
byte bytesRecvd = 0;
const char startMarker = '<';
const char endMarker = '>';

//transceiver objects
int received_data;

//flow control
bool idle = true;                       //idle waiting for master device to interact
bool COLLECTION = false;                //device wants to collect data
bool ACK_RECIEVED = false;              //
bool PARAM_SET = false;                 //Ready to set the collection params
bool PARAM_RECEIVED = false;            //Params have been set
bool sent = false;                      //data sent?


void setup()
{
  Serial.begin(115200);
  delay(1000);
  //Serial.println("0x%x", addresses[0]);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_2MBPS ) ; 

  myRadio.enableAckPayload();
  myRadio.setRetries(5,5);
  
  myRadio.openWritingPipe( addresses[0]);
  myRadio.openReadingPipe(1, addresses[1]);
  
  delay(1000);
}

void loop()
{
  int waitTime = 20;
  //set idle condition -  when its waiting for a command
  while(idle){
    GetDataFromPC();
    if(COLLECTION == true){
      //start collection
      //send an instruction

      //now need to recieve a setup configuration from the master machine
      Serial.println("ack");
      
      unsigned long waitStart = millis();
      while(true){
        if(millis() - waitStart > waitTime*1000){
            Serial.println("Timeout");
            return;
        }
        else if(Serial.available() > 0){
          //something waiting in recieved buffer
          GetDataFromPC();
          if(PARAM_SET == true){ 
            //master wwants to set the params. send ack to acknowledge
            Serial.println("ack");
            waitStart = millis();
            GetDataFromPC();
            if(PARAM_RECEIVED == true){
              //PARAM_RECEIVED = false;
              PARAM_SET = false;
              idle = false;
              break;
            }
          }
        }
      }
     }
    }
    //Serial.println("Communicating with receiver");
    //communication with transceiver - set as transmitter
    Serial.println("debug 1");
    inst.instr = 'b';
    sent = myRadio.write(&inst, sizeof(inst));
    if(sent){
      if(myRadio.isAckPayloadAvailable()){
        Serial.println("Recieved Ack");
      }
    }
    myRadio.startListening();
    delay(200);    
    Serial.println("Should be receiving");

    
    /*while(true){
      myRadio.read(&data, sizeof(data));
      received_data = data.val;
      Serial.println(data.val);
    }*/   
}


/*
* Methods for receiving and processing data from pc
*/
void GetDataFromPC(){
  //recieve data from pc. This is for commands to start stop collection

  if(Serial.available() > 0){
    char x = Serial.read();

    if (x == endMarker) {
      //for debug
      Serial.println(x);
      //
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      Serial.println("Parsing");
      parseData();
    }
    
    if(readInProgress) {
      //for debug
      Serial.println(x);
      //
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) { 
      //for debug
      Serial.println(x);
      //
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
}
void parseData() {

    // split the data into its parts
    
  char * token; // this is used by strtok() as an index
  
  token = strtok(inputBuffer,",");      // get the first part - the string
  //strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

  while(token != NULL){
    //Serial.println(token[0]);
    if(strcmp(&token[0], "command") == 0){
      //flow for recieving a command
      token = strtok(NULL, ",");
      if(strcmp(&token[0], "s") == 0){
        //start the data collection
        COLLECTION = true;        
      }
      else if(strcmp(&token[0], "a") == 0){
        //recieved an ack
        
      }
      else if(strcmp(&token[0], "pe") == 0){
        //end the params collection
        PARAM_SET = false;
        
      }
      else if(strcmp(&token[0], "p") == 0){
        //set the params
        PARAM_SET = true;
      }
      else{
        //recieved a bogus command
        
      }
    }
    else if(strcmp(&token[0], "param") == 0){
      token = strtok(NULL, ",");
      if(strcmp(&token[0], "time") == 0){
        token = strtok(NULL, ",");
        rfConfig.collectionTime = atoi(token);
        PARAM_RECEIVED = true;
        //debug
        Serial.println("Collection time set");
        Serial.println(rfConfig.collectionTime);
      }
    }
    else{
      break;
      }
  }
}
