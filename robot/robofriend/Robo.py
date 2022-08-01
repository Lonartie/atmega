import time
import serial


class Robo:

    def __init__(self):
        self.con = serial.Serial(
            port='/dev/rfcomm0', baudrate=9600, timeout=0.25,
            parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, )
        self.con.write(('HelloWorld\r').encode('ascii', 'replace'))
        self.con.flushOutput()

        buffer = ""
        while True:
            character = self.con.read(1)
            if (character != '\r' and character != '\n'):
                buffer += character
            else:
                break
        self.answer = buffer

    def close(self):
        self.con.close()
