import serial


class Robo:

    def __init__(self):
        self.con = serial.Serial(
            port='/dev/rfcomm0', baudrate=9600, timeout=0.25)
        self.con.write(b'PY\r')
        self.con.flush()
        self.answer = str(self.con.readline())

    def close(self):
        self.con.close()
