from email import message
import tkinter as tk
from tkinter import ttk, messagebox
from Robo import Robo


class Window:

    def __init__(self):
        """
        Creates the window with all the buttons
        """
        self.root = tk.Tk()
        self.root.title("RoboFriend")
        self.robot = Robo()
        self.valid = self.robot.valid
        self.setup_ui()
        self.check_connection()

    def setup_ui(self):
        """
        Builds the ui by placing buttons labels etc.
        """
        tk.Button(self.root, text="drive forward",
                  command=self.robot.drive_forward, ).pack()
        tk.Button(self.root, text="drive backward",
                  command=self.robot.drive_backward).pack()
        tk.Button(self.root, text="drive left",
                  command=self.robot.drive_left).pack()
        tk.Button(self.root, text="drive right",
                  command=self.robot.drive_right).pack()
        tk.Button(self.root, text="stop",
                  command=self.robot.drive_stop).pack()
        tk.Button(self.root, text="reset",
                  command=self.robot.reset).pack()
        pass

    def run(self):
        """
        Runs the windows main-loop
        """
        self.root.mainloop()

    def check_connection(self):
        if (not self.valid):
            messagebox.showerror(
                "Connection failure",
                "Not able to connect to the robot. Please connect the robot to '/dev/rfcomm0'!")

    def close(self):
        """
        Closes any connections to the robot
        """
        self.robot.close()
