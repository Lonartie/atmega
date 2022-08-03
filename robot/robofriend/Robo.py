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

    def clear_buffers(self):
        pass
        # self.con.flushInput()
        # self.con.flushOutput()
        # self.con.read_all()

    def drive_forward(self) -> bool:
        self.clear_buffers()
        self.send_command('fw')
        return ("OK" in self.receive_all())

    def drive_backward(self) -> bool:
        self.clear_buffers()
        self.send_command('bw')
        return ("OK" in self.receive_all())

    def drive_left(self) -> bool:
        self.clear_buffers()
        self.send_command('lt')
        return ("OK" in self.receive_all())

    def drive_right(self) -> bool:
        self.clear_buffers()
        self.send_command('rt')
        return ("OK" in self.receive_all())

    def drive_stop(self) -> bool:
        self.clear_buffers()
        self.send_command('stop')
        return ("OK" in self.receive_all())

    def reset(self) -> bool:
        self.clear_buffers()
        self.send_command('reset')
        return ("OK" in self.receive_all())

    def servo(self, angle: int) -> bool:
        self.clear_buffers()
        self.send_command('sv:' + str(angle))
        return ("OK" in self.receive_all())

    def send_command(self, command: str):
        print("sending command: '", command, "'", command)
        self.con.write(str.encode(command + "\r\n"))
        # time.sleep(0.75)

    def receive_line(self) -> str:
        return self.con.readline().decode().strip()

    def receive_all(self) -> str:
        return self.con.read_all().decode().strip()

    def close(self):
        """
        Closes any connections
        """
        if (self.valid):
            self.con.close()
