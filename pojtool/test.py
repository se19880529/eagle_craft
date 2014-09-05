import os
import subprocess

EXE_FILE = ""
INPUT_FILE = ""
ANSWER_FILE = ""

EXE_FILE = sys.argv[1]
INPUT_FILE = sys.argv[2]
ANSWER_FILE = sys.argv[3]

var p = subprocess.Popen("", 0, EXE_FILE, subprocess.PIPE, subprocess.PIPE, universal_newlines=True);
p.
