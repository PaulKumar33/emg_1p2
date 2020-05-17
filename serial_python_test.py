import numpy as np 
import time
import serial
import struct
import matplotlib.pyplot as plt

'''com = "COM15"
baud = 115200

ser = serial.Serial(com, baud,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout=0.1,
    inter_byte_timeout=0.1,
    rtscts = True
    )

res = bytearray(4)
#ser.readinto(res)


rxbuffer =b''
ser.flushInput()
ser.flushOutput()
time.sleep(2)
tik = time.time()
rec = []
while(time.time() - tik < 1):
    rxbuffer = ser.read(2)
    if(int.from_bytes(rxbuffer, 'big', signed=True) > 1023):
        print("out of sync")
        ser.flushInput()
        ser.flushOutput()
        time.sleep(0.1)
        ser.read(1)
        tik = time.time()
        continue    
    #conv = int.from_bytes(rxbuffer, 'big', signed=True)
    rec.append(int.from_bytes(rxbuffer, 'big', signed=True))
    
print(len(rec))
ser.close()

x = np.linspace(0,1,len(rec))
plt.plot(x, rec)
plt.show()'''



class serialInterfaceArduino:


    def __init__(self, com_address, baud, timeout_inter_byte = 0.1, timeout = 0.1):
        self.ser = serial.Serial(com_address, baud,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=timeout,
        inter_byte_timeout=timeout_inter_byte)
        '''rtscts=True)'''

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


    def __init__device(self):
        self.ser.flushInput()
        self.ser.flushOutput()
        time.sleep(4)

    def sendCommand(self, command = 'start'):
        try:
            self.ser.write(b'{}'.format(commands[command]))
        except KeyError:
            print(f"{command} not a valid command")

    def writeArduino(self):
        self.ser.write(b's')
        print("Command sent")
        print(self.ser.readline().decode('utf-8'))

    def readArduino(self):
        pass


if __name__=="__main__":
    ser = serialInterfaceArduino("COM15", 9600)
    ser.writeArduino()
    ser.readArduino()



