import time
import serial


class Robo:

    def __init__(self):
        self.con = serial.Serial(
            port='/dev/rfcomm0', baudrate=9600, timeout=0.25,
            parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)
        self.con.write(b'PY\r\n')
        time.sleep(3)
        answer = str(self.con.readall())
        self.long = answer
        lines = answer.split('\n')
        lines = list(filter(None, lines))
        lines = list(filter(bool, lines))
        lines = list(filter(len, lines))
        self.answer = answer[-1]

    def close(self):
        self.con.close()
