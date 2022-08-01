import tkinter as tk
from tkinter import ttk
from Robo import Robo

class Window:
  
  def __init__(self):
    self.root = tk.Tk()
    self.root.title("RoboFriend")
    self.robot = Robo()
    tk.Label(self.root, text=self.robot.con.name).pack()

  def run(self):
    self.root.mainloop()