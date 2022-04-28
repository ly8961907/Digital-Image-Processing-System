import cv2
import numpy as np
from matplotlib import pyplot as plt
#高通滤波只保留高频的像素，频谱图就是图像梯度的分布图，
#读取图像
img = cv2.imread('E:\\NEWQT\\build-DigitalImageProcessing-Desktop_Qt_5_13_0_MinGW_64_bit-Debug\\ki.jpg')
img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
#傅里叶变换
fft = cv2.dft(np.float32(img), flags=cv2.DFT_COMPLEX_OUTPUT)
fshift = np.fft.fftshift(fft)
#显示傅里叶变换的图
fly = np.fft.fft2(img)
flyshift = np.fft.fftshift(fly)
res1 = np.log(np.abs(flyshift))

#设置高通滤波器
rows, cols = img.shape
crow,ccol = int(rows/2), int(cols/2)
mask = np.ones_like(fshift, np.uint8)
mask[crow - 30:crow + 30, ccol - 30:ccol + 30] = 0#中间小矩形变成白色的

#使用高通滤波
f = fshift * mask
#画高通的频谱图
spectrum = np.log(cv2.magnitude(f[:, :, 0], f[:, :, 1]))

#傅里叶逆变换
ishift = np.fft.ifftshift(f)
iimg = cv2.idft(ishift)
iimg = cv2.magnitude(iimg[:, :, 0], iimg[:, :, 1])

#显示原始图像和高通滤波处理图像
plt.subplot(141), plt.imshow(img, 'gray'), plt.title('Original Image')
plt.axis('off')
plt.subplot(142), plt.imshow(res1, 'gray'), plt.title('FFT Image')
plt.axis('off')
plt.subplot(143), plt.imshow(spectrum, 'gray'), plt.title('Spectrum Image')
plt.axis('off')
plt.subplot(144), plt.imshow(iimg, 'gray'), plt.title('Result Image')
plt.axis('off')
plt.show()
