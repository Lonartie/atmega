from email import message
import tkinter as tk
from tkinter import ttk, messagebox
from Robo import Robo


class Window:

    def __init__(self):
        self.root = tk.Tk()
        self.root.title("RoboFriend")
        self.robot = Robo()
        tk.Label(self.root, text=self.robot.answer).pack()
        tk.Button(self.root, text='long', command=self.show_long).pack()

    def show_long(self):
        messagebox.showinfo("LONG", self.robot.long)

    def run(self):
        self.root.mainloop()
