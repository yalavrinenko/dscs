import sys
import time
import os

ship_file = sys.argv[1]

def print_file(filepath):
    print("{0}:".format(filepath))
    with open(filepath, 'r') as f:
        content = f.read().split('EOF')[0]
        print(content)

while (1):
    print_file(ship_file)

    time.sleep(0.1)
    os.system("clear")
