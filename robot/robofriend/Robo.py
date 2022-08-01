import time
import serial


class Robo:

    def __init__(self):
        self.con = serial.Serial(
            port='/dev/rfcomm0', baudrate=9600, timeout=0.25,
            parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)
        self.con.write(b'PY\r\n')
        time.sleep(3)
        answer = self.con.readall().decode()
        self.answer = answer.strip()

    def close(self):
        self.con.close()
