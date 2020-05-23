import numpy as np 
import time
import serial
import struct
import matplotlib.pyplot as plt

com = "COM16"
baud = 1000000
collect = False
complete = False
rxbuffer = ''

ser = serial.Serial(com, baud,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout=0.1,
    inter_byte_timeout=0.1)
    #rtscts = True
    #)
'''def waitForArduino():

    # wait until the Arduino sends 'Arduino is ready' - allows time for Arduino reset
    # it also ensures that any bytes left over from a previous message are discarded
    
    print("Waiting for Arduino to reset")
    
    msg = ""
    while msg.find("Arduino is ready") == -1:
        msg = recvLikeArduino()
        if not (msg == 'XXX'):
            print(msg)

def recvLikeArduino():
    if(ser.inWaiting()>0):
        x=ser.read.decode("utf-8")

        if(x)'''





ser.flushInput()
ser.flushOutput()
time.sleep(2)

rec = []
#while(time.time() - tik < 1):
'''rxbuffer = ser.read(2)
    if(int.from_bytes(rxbuffer, 'big', signed=True) > 1023):
        print("out of sync")
        ser.flushInput()
        ser.flushOutput()
        time.sleep(0.1)
        ser.read(1)
        tik = time.time()
        continue    
    #conv = int.from_bytes(rxbuffer, 'big', signed=True)
    rec.append((int.from_bytes(rxbuffer, 'big', signed=True) - 512)/1023 * 4.97)'''

    
tik = time.time()
while(time.time() - tik < 1):
    try:
        bytes = ser.read().decode()
        if(bytes == ">"):
            collect = False
            rec.append((int(rxbuffer)-512)/1023*4.97)
            rxbuffer = ''
        if(collect):
            rxbuffer += bytes
        if(bytes == "<"):
            collect = True

        
    except Exception as e:
        print(e)
        print(bytes)
            
print(len(rec))
ser.close()
rec = rec[4:]

x = np.linspace(0,1,len(rec))
plt.plot(x, rec)
plt.show()



'''class serialInterfaceArduino:


    def __init__(self, com_address, baud, timeout_inter_byte = 0.1, timeout = 0.1):
        self.ser = serial.Serial(com_address, baud,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=timeout,
        inter_byte_timeout=timeout_inter_byte)
        rtscts=True)

        print("initializing objects")
        self.recieved_bytes = []
        self.recieved_packet = b''

        self.__init__device()


        self.commands = {
        'start':'s',
        'ack':'a',
        'end':'e',
        'kill':'k'
        }
        self.com = com_address
        self.baud = baud


    def __init__device(self):
        self.ser.flushInput()
        self.ser.flushOutput()
        time.sleep(4)

    def sendCommand(self, command = 'start', params={}):
        list_params = []
        try:
            #format command
            if(command in self.commands):
                command = "<command,{}>".format(self.commands[command])
            elif(command == "params"):
                command = "<command,p>"
                for param in params.keys():
                    el = "<param,{0},{1}>".format(param, params[param])
                    list_params.append(el)
        except KeyError:
            print(f"{command} not a valid command")

        #send payload
        self.writeArduino(command, len(command))
        if(command == "params"):
            pass



    def writeArduino(self, char, len_char):
        index = 0
        while(index < len_char):
            try:
                if(self.ser.write(char[index].encode()) == 1):
                    index += 1
                    #print(self.ser.readline().decode("utf-8"))
                else:
                    raise Exception("Data could not be sent on {}".format(self.com))
                    break
            except IndexError:
                print("Index of desired data exceeded")
            except Exception as e:
                print(e)

    def readArduino(self):
        pass
'''

'''if __name__=="__main__":
    ser = serialInterfaceArduino("COM15", 9600)
    ser.sendCommand("start")
    #ser.writeArduino()
    #ser.readArduino()
'''


