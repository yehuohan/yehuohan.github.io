#f/ssr/bin/env python3
# -*- coding: utf-8 -*-

import os
import numpy as np
from scipy.io import wavfile

sample_rate = 16000     # Hz
frequency = 8000        # Hz
length = 3              # Second

t = np.linspace(0, length, sample_rate * length, dtype=np.float64, endpoint=False)
y = np.array(32767*np.sin(frequency * 2.0 * np.pi * t), dtype=np.int16)
filepath = 'sin_' + str(frequency) + 'Hz.wav'
# np.savetxt(filepath + '.txt', y, fmt='%d')
wavfile.write(filepath, sample_rate, y)
print(filepath)
