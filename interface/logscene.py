import sys
from matplotlib import pyplot as plt
import matplotlib.animation as anim

def show_file(filepath, ax):
    def update(i):
        t, px, py, vx, vy, names = [[], [], [], [], [], []]
        with open(filepath, 'r') as f:
            content = f.read().split('EOF')[0]
            lines = content.split('\n ')
            for line in lines:
                splitted_line = [a  for a in line.split(' ') if a != '']
                values = [float(v) for v in splitted_line[:-1] if v != '']

                if (values != []):
                    t.append(values[0])
                    px.append(values[1])
                    py.append(values[2])
                    vx.append(values[3])
                    vy.append(values[4])
                    names.append(splitted_line[-1])
        ax.clear()
        ax.quiver(px, py, vx, vy, scale_units='xy')

        for i, txt in enumerate(names):
            ax.annotate(txt, (px[i], py[i]))
        plt.title(t[0])
    return update

fig = plt.figure()
ax = fig.add_subplot(1,1,1)

update_func = show_file(sys.argv[1], ax)

a = anim.FuncAnimation(fig, update_func, frames=100, repeat=True, interval=100)
plt.show()
