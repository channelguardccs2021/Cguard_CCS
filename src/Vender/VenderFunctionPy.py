#VendorFunction.py is implemented by vendor
#RPi.GPIO
import sys
import time
import RPi.GPIO as GPIO

pina = 23 #RPi.GPIO 23 Eighth on the upper right

class VenderPy():
    def __init__(self):
        super().__init__()

        pina = 23 #rpi23
        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(pina, GPIO.OUT)
        
    def OperateDevicePy(self,value):
        pina = 23 #rpi23

        print("****Gadget Set light status to %d ****" % value)
        
        if(value == 1):
            GPIO.output(pina, GPIO.HIGH)
            time.sleep(1)

        elif(value == 0):
            GPIO.output(pina, GPIO.LOW)
            time.sleep(1)

        #VenderLED.close()
        

    def ReadDeviceStatusPy(self):
        print("****Gadget Set light status to %d ****" % value)
        return value

if __name__ == '__main__':
    venderledtest = VenderPy()
    venderledtest.OperateDevicePy(1)
    #time.sleep(2)
    #venderledtest.OperateDevicePy(0)
    #time.sleep(2)
