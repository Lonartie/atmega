import time
import serial


class Robo:

    def __init__(self):
        self.con = serial.Serial(
            port='/dev/rfcomm0', baudrate=9600, timeout=0.25)
        self.con.write(('HelloWorld\r').encode('ascii', 'replace'))

    def close(self):
        self.con.close()
