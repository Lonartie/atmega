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
        self.con.flushInput()
        self.con.flushOutput()

    def drive_forward(self) -> bool:
        self.clear_buffers()
        self.send_command('fw')
        return (self.receive_line() == "OK")

    def drive_backward(self) -> bool:
        self.clear_buffers()
        self.send_command('bw')
        return (self.receive_line() == "OK")

    def drive_left(self) -> bool:
        self.clear_buffers()
        self.send_command('lt')
        return (self.receive_line() == "OK")

    def drive_right(self) -> bool:
        self.clear_buffers()
        self.send_command('rt')
        return (self.receive_line() == "OK")

    def drive_stop(self) -> bool:
        self.clear_buffers()
        self.send_command('stop')
        return (self.receive_line() == "OK")

    def reset(self) -> bool:
        self.clear_buffers()
        self.send_command('reset')
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
