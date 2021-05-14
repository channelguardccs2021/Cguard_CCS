#
# Copyright 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
# These materials are licensed under the Amazon Software License in connection with the Alexa Gadgets Program.
# The Agreement is available at https://aws.amazon.com/asl/.
# See the Agreement for the specific terms and conditions of the Agreement.
# Capitalized terms not defined in this file have the meanings given to them in the Agreement.
#

import logging
import sys
import time
from gpiozero import LED

sys.path.append("../../src/Vender")
from VenderFunctionPy import VenderPy
sys.path.append("../../lib/Alexa-Gadgets-Raspberry-Pi-Samples")
from agt import AlexaGadget

#GPIO_PIN = 23 #gpiozero gpio23 Sixth on the upper right
logging.basicConfig(stream=sys.stdout, level=logging.INFO)
logger = logging.getLogger(__name__)

led_state  = 0 #led init state
led_lock   = 0

#LED = LED(GPIO_PIN)


class WakewordGadget(AlexaGadget):
    """
    An Alexa Gadget that turns an LED on and off in sync with the detection
    of the wake word
    """

    def __init__(self):
        super().__init__()

    def on_alexa_gadget_statelistener_stateupdate(self, directive):
        global led_state
        global led_lock
        VenerLed = VenderPy()
        fo = open("gadget_led", "w+")#open file

        for state in directive.payload.states:
            if state.name == 'wakeword':
                if state.value == 'active':
                    if(led_lock == 0):

                        if led_state == 0:
                            led_lock = 1#lock on

                            logger.info('Echo Wake word active - turn on LED')
                            led_state = 1
                            #VenerLed.OperateDevicePy(led_state)# led on and sleep(1)
                            #gadget write file
                            fo.write("1")#set file to 1
                            fo.seek(0)
                            fo.flush()#flush disk
                            

                            led_lock = 0#lock off
                            continue 

                        if led_state == 1:
                            led_lock = 1#lock on

                            logger.info('Echo Wake word active - turn off LED')
                            led_state = 0
                            #VenerLed.OperateDevicePy(led_state)# led on and sleep(1)
                            #gadget write file
                            fo.write("0")#set file to 1
                            fo.seek(0)
                            fo.flush()#flush disk
                            
                            led_lock = 0#lock off
                            continue
        
        fo.close()#close fiule



if __name__ == '__main__':
    try:
        WakewordGadget().main()
    finally:
        logger.debug('Cleaning up GPIO')
        #LED1 = LED(23)
        #LED1.close()
