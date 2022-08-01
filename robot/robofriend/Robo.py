import time
import serial


class Robo:

    def __init__(self):
        self.con = serial.Serial(
            port='/dev/rfcomm0', baudrate=9600, timeout=0.25)

        self.con.flushInput()
        self.con.flushOutput()

        self.con.write('PY\r'.encode("uft-8"))

        time.sleep(1)

        self.answer = str(self.con.readline())

    def close(self):
        self.con.close()
