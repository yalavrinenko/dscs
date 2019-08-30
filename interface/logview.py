import glob
import time
import os

def print_file(filepath):
    print("{0}:".format(filepath))
    with open(filepath, 'r') as f:
        content = f.read().split('EOF')[0]
        print(content)

while (1):
    file_list = glob.glob("../ramfs/*")
    for file in file_list:
        print_file(file)

    time.sleep(0.1)
    os.system("clear")
