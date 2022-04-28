import cv2
import numpy as np
from matplotlib import pyplot as plt
#高通滤波只保留高频的像素
#读取图像
img = cv2.imread('E:\\NEWQT\\build-DigitalImageProcessing-Desktop_Qt_5_13_0_MinGW_64_bit-Debug\\ki.jpg', 0)

#DCT变换
dct = cv2.dct(np.float32(img))
res1 = np.log(np.abs(dct))

#设置高通滤波器
rows, cols = img.shape
crow,ccol = int(rows/2), int(cols/2)
mask = np.ones_like(dct, np.uint8)
mask[crow - 30:crow + 30, ccol - 30:ccol + 30] = 0

#使用高通滤波
f = dct * mask
#高通频谱图
spectrum = np.log(np.abs(f))

#DCT逆变换
iimg = cv2.idct(dct)
iimg = np.abs(iimg)

#显示原始图像和高通滤波处理图像
plt.subplot(141), plt.imshow(img, 'gray'), plt.title('Original Image')
plt.axis('off')
plt.subplot(142), plt.imshow(res1, 'gray'), plt.title('DCT Image')
plt.axis('off')
plt.subplot(143), plt.imshow(spectrum, 'gray'), plt.title('Spectrum Image')
plt.axis('off')
plt.subplot(144), plt.imshow(iimg, 'gray'), plt.title('Result Image')
plt.axis('off')
plt.show()
