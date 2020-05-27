#include "SerialPort.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <time.h>
using namespace std;

bool connected = false;
//DCB dcbSerialParameters;
HANDLE handler;
TCHAR *pcCommPort = TEXT("\\\\.\\COM14");
DWORD errors;
COMSTAT comStatus;
char read_data[1];
char hold_data[1];


void SerialPort(char *portName)
{
    

    handler = CreateFileA(pcCommPort,
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);
    if (handler == INVALID_HANDLE_VALUE){
        if (GetLastError() == ERROR_FILE_NOT_FOUND){
            printf("ERROR: Handle was not attached. Reason: %s not available\n", portName);
        }
    else
        {
            printf("ERROR!!!");
        }
    }
    else {
        DCB dcbSerialParameters = {0};
        printf("Successfull connection made on Port: %s\n", pcCommPort);
        connected = true;

        if (!GetCommState(handler, &dcbSerialParameters)) {
            printf("failed to get current serial parameters");
        }
        else {
            dcbSerialParameters.BaudRate = CBR_115200;
            dcbSerialParameters.ByteSize = 8;
            dcbSerialParameters.StopBits = ONESTOPBIT;
            dcbSerialParameters.Parity = NOPARITY;
            dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(handler, &dcbSerialParameters))
            {
                printf("ALERT: could not set Serial port parameters\n");
            }
            else {
                connected = true;
                PurgeComm(handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
                Sleep(ARDUINO_WAIT_TIME);
            }
        }
    }
}

int ReadSerialPort(char *buffer, unsigned int buf_size){
    DWORD bytesRead;
    unsigned int toRead = 0;

    ClearCommError(handler, &errors, &comStatus);
    if(comStatus.cbInQue>0){
        if(comStatus.cbInQue > buf_size){
            toRead = buf_size;
        }
        else{
            toRead = comStatus.cbInQue;
        }
        if(ReadFile(handler, buffer, toRead, &bytesRead, NULL)){
            return bytesRead;
        }
        return 0;
    }
    return 0;
}

char* PollSerialPort(){
    //this method polls the serial port
}

int main(){
    SerialPort("\\.\\\\COM14");
    if(connected){
        bool startByte = false;
        bool stopByte  = false;
        bool startCollection = false;
        ReadSerialPort(hold_data, sizeof(hold_data));
        printf("Waiting for start bit\n");
        while(hold_data[0] != '<'){
            ReadSerialPort(hold_data, sizeof(hold_data));
            if(hold_data[0] == '<'){
                printf("Start bit\n");
                break;
            }
        }
        int _speed = 0;
        int counter = 0;
        clock_t t;
        t = clock();
        while(true){
            //wait for return statement
            
            ReadSerialPort(read_data, sizeof(read_data));
            
            if(read_data[0] == '<'){
                //printf("Start command\n");
                startByte = true;
                stopByte = false;
                startCollection = false;
                continue;
            }
            if(read_data[0] == '>'){
                stopByte = true;
                startByte = false;
                startCollection = false;
                printf("\n");
                continue;
            }
            if(read_data[0] != '<' || read_data[0] != '>'){
                startCollection= true;
            }
            if(startCollection){
                //printf("%c", read_data[0]);
                _speed++;
                //printf("%d\n", _speed);
            }
            while(counter < 4000){
                counter ++;
            }
            counter =0;
            //Sleep(1);
            //if all else passes print the read value
            if(_speed == 30000){
                t = clock() - t;
                break;

            }                     
        }
        cout << "time: " << t << "ms" << endl;          
    }
}


