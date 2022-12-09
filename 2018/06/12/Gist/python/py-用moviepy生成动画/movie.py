
import numpy as np
import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d as plt3d
import moviepy.editor as mpy
from moviepy.video.io.bindings import mplfig_to_npimage

dura = 2    # 2s动画

fig = plt.figure("movie", figsize=(5, 3))
ax = fig.add_subplot(111, projection='3d')
time = np.linspace(-2*np.pi, 2*np.pi, 1000)
x,y = np.meshgrid(time, time)
z = np.sin(np.sqrt(x**2 + y**2))
ax.plot_surface(x, y, z, cmap='jet')

def make_frame(t):
    """生成时间t的gif帧数据
    :Parameters: 
        - t: t时刻，其范围从0开始，到设置的gif时间长度
    :Returns: 
        返回Numpy array表示的图像数据（Height x Width x 3），
        绘制图像可以用任何工具，只要返回满足格式要求。
    """
    ax.view_init(elev=50, azim=t*180)
    return mplfig_to_npimage(fig) # RGB image of the figure

anim = mpy.VideoClip(make_frame, duration=dura)
anim.write_videofile("3d.mp4", fps=24)  # 生成mp4视频
anim.write_gif("3d.gif", fps=24)        # 生成gif动画
