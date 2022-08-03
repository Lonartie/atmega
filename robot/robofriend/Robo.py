import time
import serial


class Robo:
    """
    Robo class to talk to the robot through a serial bus
    """

    def __init__(self):
        """
        Initializes the robo instance by connecting to the robot and start the
        PY-Mode of the robot.
        Ask for successul connection through instance.valid
        """
        try:
            self.con = serial.Serial(
                port='/dev/rfcomm0', baudrate=9600, timeout=0.25,
                parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE)
            self.send_command('PY')
            time.sleep(3)
            self.valid = (self.receive_line() == "OK")
        except:
            self.valid = False

    def drive_forward(self) -> bool:
        self.send_command('fw')
        time.sleep(1)
        return (self.receive_line() == "OK")

    def send_command(self, command: str):
        self.con.write(str.encode(command + "\r\n"))

    def receive_line(self) -> str:
        return self.con.readline().decode().strip()

    def close(self):
        """
        Closes any connections
        """
        if (self.valid):
            self.con.close()
