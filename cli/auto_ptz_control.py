# Mehmet cagri Aksoy - 2024
# Automated Software control of a Pelco-D PTZ camera using Python

import serial
from serial.tools import list_ports
import keyboard
import random
import time

#clear the terminal
import os
os.system('cls' if os.name == 'nt' else 'clear')

# Print the instructions
print("\033[96m")
print("==================================")
print("Pelco-D PTZ Camera Control Script")
print("==================================")
print("This script allows you to control a \n Pelco-D PTZ camera using the arrow keys on your keyboard")
print("==================================")
print("Author: Mehmet cagri Aksoy - 2024")
print("==================================")
print("\033[0m")
print("THIS SCRIPT RUNNING AUTOMATICALLY")
print("==================================")
# Open serial port

print("Please select the serial port:")

# Display the available serial ports
ports = list_ports.comports()
for port, desc, hwid in sorted(ports):
    print("{}: {}".format(port, desc))

# Open the selected serial port
#ser = serial.Serial('COM17', 9600)  # change the second argument to your baud rate

# get the port from user
port = input("Enter the port number: ")
ser = serial.Serial("COM"+port, 9600)  # change the second argument to your baud rate
if ser.isOpen():
    print("Port status: Serial port is open and connected!")
else:
    print("\033[91m")
    print("Port status: Serial port is not open and connected!")
    print("\033[0m")

print("Do not do anything, the script will run automatically!")

# Define the functions to send the Pelco commands

def send_pelco_up(e):
    # send the Pelco up command over the serial port
    string = 'FF010008101029'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

    print('\r I am moving UP!     ', end='', flush=True)

def send_pelco_down(e):
    # send the Pelco down command over the serial port
    string = 'FF010010101031'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

    print('\r I am moving DOWN!    ', end='', flush=True)

def send_pelco_left(e):
    # send the Pelco left command over the serial port
    string = 'FF010004121027'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

    print('\r I am moving LEFT!    ', end='', flush=True)

def send_pelco_right(e):
    # send the Pelco right command over the serial port
    string = 'FF010002132036'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

print('\r I am moving RIGHT!   ', end='', flush=True)

def send_pelco_zoom_in(e):
    # send the Pelco zoom in command over the serial port
    string = 'FF010020000021'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

    print('\r I am zooming IN!     ', end='', flush=True)

def send_pelco_zoom_out(e):
    # send the Pelco zoom out command over the serial port
    string = 'FF010040000041'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

    print('\r I am zooming OUT!    ', end='', flush=True)

def send_pelco_focus_near(e):
    # send the Pelco focus near command over the serial port
    string = 'FF010080000081'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

    print('\r I am focusing NEAR!  ', end='', flush=True)

def send_pelco_focus_far(e):
    # send the Pelco focus far command over the serial port
    string = 'FF010100000103'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

    print('\r I am focusing FAR!   ', end='', flush=True)

def send_enter(e):
    # send the Enter button press over the serial port
    ser.write('\r'.encode())
    #time.sleep(0.3)

def send_pelco_up_right(e):
    # send the Pelco up right command over the serial port
    string = 'FF01000A13203E'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

    print('\r I am moving UP RIGHT!     ', end='', flush=True)

def send_pelco_up_left(e):
    # send the Pelco up left command over the serial port
    string = 'FF01000C12102F'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

    print('\r I am moving UP LEFT!     ', end='', flush=True)

def send_pelco_down_right(e):
    # send the Pelco down right command over the serial port
    string = 'FF010012132046'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

    print('\r I am moving DOWN RIGHT!     ', end='', flush=True)

def send_pelco_down_left(e):
    # send the Pelco down left command over the serial port
    string = 'FF010014121037'
    bytes_to_send = bytes.fromhex(string)
    ser.write(bytes_to_send)

    print('\r I am moving DOWN LEFT!     ', end='', flush=True)

while True:
    if keyboard.is_pressed('q'):  # if 'q' is pressed, quit
        break
    else:
        random.choice([send_pelco_focus_near, send_pelco_focus_far])(None)

        # Wait for 15 seconds
        time.sleep(15)

        # Randomly call zoom in or out
        random.choice([send_pelco_zoom_in, send_pelco_zoom_out])(None)

        time.sleep(0.3) # sleep for 0.3 seconds RULE!

# Close serial port
ser.close()