from matplotlib import pyplot as plt
import matplotlib.animation as anim

ranges = {}

def show_file(filepath, ax):
    def update(i):
        tmp_ranges = {}
        with open(filepath, 'r') as f:
            content = f.read().split('EOF')[0]
            lines = content.split('\n ')
            for line in lines[2::]:
                splitted_line = [a  for a in line.split(' ') if a != '']
                values = [float(v) for v in splitted_line[1::] if v != '']

                if (values != []):
                    rid = values[0]
                    if rid in tmp_ranges:
                        tmp_ranges[rid].append([values[1], values[2]])
                    else:
                        tmp_ranges[rid] = [[values[1], values[2]], ]


        for k, v in tmp_ranges.items():
            ranges[k] = v

        ax.clear()
        for k, v in ranges.items():
            ax.plot(map(lambda p: p[1], v), map(lambda p: p[0], v), 'rv')
    return update

fig = plt.figure()
ax = fig.add_subplot(1,1,1, projection='polar')

import sys
update_func = show_file(sys.argv[1], ax)

a = anim.FuncAnimation(fig, update_func, frames=100, repeat=True, interval=100)
plt.show()
