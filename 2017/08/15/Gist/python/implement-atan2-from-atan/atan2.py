from math import atan
from math import atan2
from math import pi

# atan2 implementation from atan
# atan2(sin, cos)
def myatan2(dy, dx):
    if dx == 0:
        if dy > 0:
            return pi/2
        elif dy < 0:
            return -pi/2
    else:
        re = atan(dy / dx)
        if dx > 0:
            return re
        elif dx < 0 and dy >= 0:
            return re + pi
        elif dx < 0 and dy < 0:
            return re - pi


if __name__ == "__main__":
    dx = 3
    dy = -4
    print("atan  : {0:8f} rad".format(atan(dy/dx)))
    print("atan  : {0:8f} deg".format(atan(dy/dx)*180/pi))
    print("atan2 : {0:8f} rad".format(atan2(dy,dx)))
    print("atan2 : {0:8f} deg".format(atan2(dy,dx)*180/pi))
    print("mytan2: {0:8f} rad".format(myatan2(dy,dx)))
    print("mytan2: {0:8f} deg".format(myatan2(dy,dx)*180/pi))
