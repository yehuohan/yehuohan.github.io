
"""
python 绘制相平面轨迹
二阶方程：ddx + dx + 0.5x = 0
"""

import matplotlib.pyplot as plt
import math

x = 0           # x初始
dx = 10         # dx初值
time = 0        # 仿真时间
dt = 0.001      # 积分步长
size = 20000
p_dx = [0 for x in range(size)]
p_x = [0 for x in range(size)]

for k in range(size):
    ddx = - dx - 0.5 * x
    dx += ddx * dt
    x += dx * dt
    p_dx[k] = dx
    p_x[k] = x
    time += dt

print("Time: {}".format(time))
plt.plot(p_x, p_dx)
plt.show()
