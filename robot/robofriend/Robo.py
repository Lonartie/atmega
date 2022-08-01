import time
import serial


class Robo:

    def __init__(self):
        self.con = serial.Serial(
            port='/dev/rfcomm0', baudrate=9600, timeout=0.25)
        self.con.write(('PY\r').encode('ascii', 'replace'))
        buffer = ""
        while True:
            oneByte = self.con.read(1)
            if oneByte == b"\n":
                break
            else:
                buffer += oneByte.decode()
        self.answer = buffer

    def close(self):
        self.con.close()
