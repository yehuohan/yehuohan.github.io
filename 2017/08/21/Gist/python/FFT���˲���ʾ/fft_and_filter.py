
import numpy as np
from scipy.fftpack import fft, ifft
import matplotlib.pyplot as plt


## @brief generate sin wave
# 
#  @param x: independent variable
#  @param AF: a list array where [0] is amplitude and [1] is freqency
#  @return sin wave value
#  @retval None
def func_sin_wave(x, AF):
    sf = 0
    for var in AF:
        sf += var[0] * np.sin(2*np.pi * var[1] * x)
    return sf

## @brief discrete low pass filter
# 
#  @param a: a = Wc*T/(Wc*T+1) where Wc is the cut-off frequency
#  @param x: the input signal
#  @return filtered singal
#  @retval None
def func_low_pass_filter(a,x):
    y = []
    yl = 0
    for var in x:
        yl = a*var + (1-a)*yl
        y.append(yl)
    return y
    

def main():
    N = 3000            # 序列长度
    F = 5000            # 采样频率（>= 2倍信号频率），Hz
    time = N*(1/F)      # 信号时间长度
    # 时域自变量：时间区间为[start, stop]，采样频率为num/(stop-start)
    t = np.linspace(start=0, stop=time, num=N)
    # 时域信号：频率为 150， 700， 1000
    y = func_sin_wave(t, [[7,150], [3,700], [5,1000]])
    # 频域自变量
    x_f = np.linspace(start=0, stop=F, num=N)
    # 频域信号计算
    y_fft = fft(y)                      # fft变换
    y_fft_real = y_fft.real             # 实部与虚部
    y_fft_imag = y_fft.imag
    y_fft_abs = abs(y_fft)              # 使用abs对复数取模
    y_fft_nor = abs(y_fft)/N            # 归一化 
    y_fft_half = y_fft_nor[range(int(len(t)/2))]
    # 频域对应的自变量
    x_fft = x_f
    x_fft_nor = x_fft
    x_fft_half = x_fft[range(int(len(t)/2))]      # 单边
    # 低通滤波
    y_filter = func_low_pass_filter(200/(F+200), y)     # 截止频率200

    # 绘制FFT图
    plt.figure("FFT")
    # 绘制实部与虚部
    plt.subplot(231)
    plt.plot(x_fft, y_fft_real)
    plt.title("Real")
    plt.subplot(232)
    plt.plot(x_fft, y_fft_imag)
    plt.title("Imag")
    # 绘制原波形
    plt.subplot(233)
    plt.plot(t[0:100], y[0:100])
    plt.title("Original Wave")
    # 以频率为横坐标，绘制FFT变换图
    plt.subplot(234)
    plt.plot(x_fft, y_fft_abs, 'r')
    plt.title("FFT Wave(double range)")
    plt.subplot(235)
    plt.plot(x_fft, y_fft_nor, 'g')
    plt.title("FFT Wave(normalization)")
    plt.subplot(236)
    plt.plot(x_fft_half, y_fft_half, 'b')
    plt.title("FFT Wave(single range)")

    # 绘制滤波图
    plt.figure("Filter")
    plt.subplot(211)
    plt.plot(t[100:200], y[100:200])
    plt.subplot(212)
    plt.plot(t[100:200], y_filter[100:200])

    plt.show()

if __name__ == "__main__":
    main()
