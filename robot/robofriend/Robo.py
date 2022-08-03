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
            time.sleep(1.5)
            self.valid = ("OK" in self.receive_all())
        except:
            self.valid = False

    def clear_buffers(self):
        """
        Clears the serial input and output buffers
        """
        self.con.flushInput()
        self.con.flushOutput()
        self.con.read_all()

    def drive_forward(self):
        """
        Sends the drive forward command to the robot
        """
        self.clear_buffers()
        self.send_command('fw')

    def drive_backward(self):
        """
        Sends the drive backward command to the robot
        """
        self.clear_buffers()
        self.send_command('bw')

    def drive_left(self):
        """
        Sends the drive left command to the robot
        """
        self.clear_buffers()
        self.send_command('lt')

    def drive_right(self):
        """
        Sends the drive right command to the robot
        """
        self.clear_buffers()
        self.send_command('rt')

    def stop(self):
        """
        Sends the stop command to the robot
        """
        self.clear_buffers()
        self.send_command('stop')

    def reset(self) -> bool:
        """
        Sends the reset command to the robot
        Returns true if the robot received the command and will reset itself
        """
        self.clear_buffers()
        self.send_command('reset')
        return ("OK" in self.receive_all())

    def servo(self, angle: int):
        """
        Sends a command to the robot to point the servo into the given direction
        """
        self.clear_buffers()
        self.send_command('sv:' + str(angle))

    def send_command(self, command: str):
        """
        Sends the given command to the robot
        """
        print("sending command: '", command, "'", sep='')
        self.con.write(bytes(command + "\r\n", encoding="ascii"))
        self.con.flush()
        self.con.flushOutput()
        time.sleep(1)

    def receive_all(self) -> str:
        """
        Receives everything the robot has sent us
        """
        data = []
        while self.con.inWaiting() > 0:
            msg = self.con.readline().strip()
            time.sleep(0.1)  # Wait for buffer
            msg = msg.replace(b"\r", b"")
            msg = msg.replace(b"\n", b"")
            if msg != b"":
                data.append(str(msg, encoding='ascii'))
        return '\n'.join(data)

    def close(self):
        """
        Closes any connections
        """
        if (self.valid):
            self.con.close()
