import serial
import time


class Relay_Controller():
    # Initialize the controller
    # @param dev The device of the UART module.
    def __init__(self, dev):
        self.dev = "/dev/"+dev
        self.ser = serial.Serial(self.dev)
        self.ser.setRTS(False) # Initialize RTS
        self.ser.setDTR(False) # Initialize DTR
    def turn_computer_on(self):
        self.ser.setDTR(True)
        time.sleep(1) # how much time to let the circuit closed.
        self.ser.setDTR(False)
    def reset_computer(self):
        self.ser.setRTS(True)
        time.sleep(1)
        self.ser.setRTS(False)

user_input = int(input("Chooce action, [1] power on [2] reset: "))
relay = Relay_Controller("ttyUSB0")

if (user_input == 1):
    relay.turn_computer_on()
elif (user_input == 2):
    relay.reset_computer()
else: 
    print("Unrecognized option")
