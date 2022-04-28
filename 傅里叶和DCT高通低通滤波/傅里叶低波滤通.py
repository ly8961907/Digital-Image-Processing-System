# -*- coding: utf-8 -*-
import cv2
import numpy as np
from matplotlib import pyplot as plt

#读取图像
img = cv2.imread('E:\\NEWQT\\build-DigitalImageProcessing-Desktop_Qt_5_13_0_MinGW_64_bit-Debug\\ki.jpg', 0)

#傅里叶变换
dft = cv2.dft(np.float32(img), flags = cv2.DFT_COMPLEX_OUTPUT)
fshift = np.fft.fftshift(dft)

#设置低通滤波器
rows, cols = img.shape
crow,ccol = int(rows/2), int(cols/2) #中心位置
mask = np.zeros((rows, cols, 2), np.uint8)
mask[crow-30:crow+30, ccol-30:ccol+30] = 1#中间变成黑色

#掩膜图像和频谱图像乘积
f = fshift * mask

# 频谱图转换为可以显示的图片
spectrogram = np.log(cv2.magnitude(f[:, :, 0], f[:, :, 1]))

#傅里叶逆变换
ishift = np.fft.ifftshift(f)
iimg = cv2.idft(ishift)
iimg = cv2.magnitude(iimg[:,:,0], iimg[:,:,1])

#显示原始图像和低通滤波处理图像
plt.subplot(131), plt.imshow(img, 'gray'), plt.title('Original Image')
plt.axis('off')
plt.subplot(132), plt.imshow(spectrogram, 'gray'), plt.title('Spectrogram Image')
plt.axis('off')
plt.subplot(133), plt.imshow(iimg, 'gray'), plt.title('Result Image')
plt.axis('off')
plt.show()
