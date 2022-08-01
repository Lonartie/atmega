import time
import serial


class Robo:

    def __init__(self):
        self.con = serial.Serial(
            port='/dev/rfcomm0', baudrate=9600, timeout=0.25)
        self.con.write(('PY\r').encode('ascii', 'replace'))
        self.answer = self.con.readline().decode('ascii')

    def close(self):
        self.con.close()
