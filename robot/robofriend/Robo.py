import serial

class Robo:

  def __init__(self):
    self.con = serial.Serial('/dev/rfcomm0')
    print("connected to: " + self.con.name)

  def close(self):
    self.con.close()