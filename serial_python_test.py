import numpy as np 
import time
import serial
import struct
import matplotlib.pyplot as plt

com = "COM15"
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
plt.show()
