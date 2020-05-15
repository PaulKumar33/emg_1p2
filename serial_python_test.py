import numpy as np 
import time
import serial
import struct
import matplotlib.pyplot as plt

com = "COM3"
baud = 115200

ser = serial.Serial(com, baud,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout=0.1,
    inter_byte_timeout=0.1
    )

res = bytearray(4)
#ser.readinto(res)


rxbuffer =b''
ser.flushInput()
time.sleep(2)
tik = time.time()
rec = []
while(time.time() - tik < 1):
    rxbuffer = ser.read(2)
    conv = int.from_bytes(rxbuffer, 'big', signed=True)
    rec.append(conv)
    #print(conv)
    
    
    ##rec.append(value2)
    #print(float(value)/13107)

    #print(res)
#print(int.from_bytes(rxbuffer, 'big', signed=True))
#print(len(rxbuffer))
#print(rxbuffer)
print(len(rec))
ser.close()

x = np.linspace(0,1,len(rec))
plt.plot(x, rec)
plt.show()
