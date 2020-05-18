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

struct config{
  int collectionTime = 10;

};

//initialize structures
typedef struct package Package;
typedef struct instruction Instruction;
typedef struct config Config;
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

//flow control
bool idle = true;
bool COLLECTION = false;
bool ACK_RECIEVED = false



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
  //set idle condition -  when its waiting for a command
  while(idle){
    GetDataFromPC();
    if(COLLECTION == true){
      //start collection
      //send an instruction

      //now need to recieve a setup configuration from the master machine
      Serial.println("ack");
      int waitTime = 2;
      unsigned long waitStart = millis();
      while(true){
        if(millis() - waitStart > waitTime){
            Serial.println("Timeout");
            return;
        }
        else if(Serial.available > 0){
          //something waiting in recieved buffer
          GetDataFromPC();
        }
      }
      inst.instr = 'b';
      myRadio.write(&inst, sizeof(inst));
      myRadio.startListening();


    }

  }
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
      else if(strcmp(&token[0], "e") == 0){
        //end the data collection
        
      }
      else{
        //recieved a bogus command
        
      }
    }
    else if(strcmp(&token[0], "param") == 0){
      token = strtok(NULL, ",");
      if(strcmp(&token[0], "time") == 0){

      }
    }
    else{
      break;
      }
  }
}
