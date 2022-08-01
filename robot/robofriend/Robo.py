import serial


class Robo:

    def __init__(self):
        self.con = serial.Serial(
            port='/dev/rfcomm0', baudrate=9600, timeout=0.25)
        self.con.write(b'?\r')
        self.con.flush()
        answer = ""
        done = False
        while not done:
            ch = self.con.read(1)
            if (ch == None):
                done = True
            else:
                answer += str(ch)
        self.answer = answer

    def close(self):
        self.con.close()
