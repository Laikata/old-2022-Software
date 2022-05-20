#!/bin/env python3

# A packet parser for debugging with Laikata's packet protocol (https://github.com/Laikata/Software/blob/main/lib/comms/docs/comms.md)
import serial
import struct
import sys

if len(sys.argv) < 2:
    print("Please input port number as a parameter")
    exit(1)

port = sys.argv[1]
serial_port = serial.Serial(port, 9600);

state = 0
packet_size = 0

# Loop states:
#   READ = 0,
#   GPS = 0x01,
#   IMU = 0x02,
#   ENV = 0x03,
#   BAT = 0x04,
#   DEBUG = 0x05

while True:
    if state == 0:
        result = serial_port.read()
        if result[0] == 0x16:
            result = serial_port.read(size=3)
            identifier = result[2]
            if identifier > 5:
                raise Exception("Non-valid packet id")
            state = identifier
            packet_size = result[0]

    elif state == 0x01:
        result = serial_port.read(size=12)
        coords = [0.0, 0.0, 0.0]
        for i in range(3):
            coords[i] = struct.unpack("!f", result[i*4:i*4+4])[0]
        print(f"GPS -> lat: {coords[0]} lon: {coords[1]} alt: {coords[2]}")
        serial_port.read(size=4);
        state = 0

    elif state == 0x02:
        result = serial_port.read(size=40)
        mag = [0.0, 0.0, 0.0]
        accel = [0.0, 0.0, 0.0]
        gyro = [0.0, 0.0, 0.0]

        for i in range(3):
            mag[i] = struct.unpack("!f", result[i*4:i*4+4])[0]
        for i in range(3, 6):
            accel[i - 3] = struct.unpack("!f", result[i*4:i*4+4])[0]
        for i in range(6, 9):
            gyro[i - 6] = struct.unpack("!f", result[i*4:i*4+4])[0]

        print(f"IMU -> Mag: {mag} Accel: {accel} Gyro: {gyro}")
        serial_port.read(size=4)
        state = 0
    
    elif state == 0x03:
        result = serial_port.read(size=12)

        temp = struct.unpack("!f", result[0:4])[0]
        hum = struct.unpack("!f", result[4:8])[0]
        press = struct.unpack("!f", result[8:12])[0]

        print(f"ENV -> Temp: {temp} Hum: {hum} Press: {press}")
        serial_port.read(size=4)
        state = 0

    elif state == 0x04:
        result = serial_port.read(size=4)
        voltage = struct.unpack("!f", result)[0]

        print(f"BAT -> {voltage}V")
        serial_port.read(size=4)
        state = 0
    
    elif state == 0x05:
        message = serial_port.read(size=packet_size - 1)
        print(f"DEBUG -> {message.decode('ascii')}")
        serial_port.read(size=4)
        state = 0
