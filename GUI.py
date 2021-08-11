import tkinter
from tkinter import filedialog
import os
import subprocess


root = tkinter.Tk()
root.withdraw()

currdir = os.getcwd()
tempdir = filedialog.askopenfilename(parent=root, initialdir=currdir, title='Please select a directory')
if len(tempdir) > 0:
    proc = subprocess.Popen(["./zipper", tempdir])
    proc.wait()