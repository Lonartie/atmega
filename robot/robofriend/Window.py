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
                  command=self.robot.stop).pack()
        tk.Button(self.root, text="servo 90°",
                  command=lambda: self.robot.servo(90)).pack()
        tk.Button(self.root, text="servo 45°",
                  command=lambda: self.robot.servo(45)).pack()
        tk.Button(self.root, text="servo 0°",
                  command=lambda: self.robot.servo(0)).pack()
        tk.Button(self.root, text="servo -45°",
                  command=lambda: self.robot.servo(-45)).pack()
        tk.Button(self.root, text="servo -90°",
                  command=lambda: self.robot.servo(-90)).pack()
        tk.Button(self.root, text="reset",
                  command=self.reset).pack()

    def run(self):
        """
        Runs the windows main-loop
        """
        self.root.mainloop()

    def reset(self):
        """
        Resets the robot and if successful terminates this program
        """
        if self.robot.reset():
            messagebox.showinfo(
                "Shutdown", "The robot will reset and the application will close.")
            self.root.destroy()
        else:
            messagebox.showinfo("Failed", "Something went wrong :(")

    def check_connection(self):
        """
        Checks whether or not a connection to the robot could be established.
        Will show an error if no connection could be established
        """
        if (not self.valid):
            messagebox.showerror(
                "Connection failure",
                "Not able to connect to the robot. Please connect the robot to '/dev/rfcomm0'!")

    def close(self):
        """
        Closes any connections to the robot
        """
        self.robot.close()
