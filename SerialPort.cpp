#include "SerialPort.h"
#include <stdio.h>
#include <tchar.h>
#include <iostream>
using namespace std;

bool connected = false;
//DCB dcbSerialParameters;
HANDLE handler;
TCHAR *pcCommPort = TEXT("\\\\.\\COM14");
DWORD errors;
COMSTAT comStatus;
char read_data[255];


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
            dcbSerialParameters.BaudRate = CBR_9600;
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

int main(){
    SerialPort("\\.\\\\COM14");
    if(connected){
        ReadSerialPort(read_data, sizeof(read_data));

        cout<<">> "<<read_data<<endl;

        //to see how data is returned
        for(int el = 0; el<sizeof(read_data); el++){
            // if(read_data[el] == '\r'){
            //     printf("ping!\n");
            // }
            printf("Element %d of read buffer: %d\n", el, read_data[el]);
        }
    }
}


