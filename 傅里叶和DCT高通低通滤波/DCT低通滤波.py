import cv2
import numpy as np
from matplotlib import pyplot as plt

#读取图像
img = cv2.imread('E:\\NEWQT\\build-DigitalImageProcessing-Desktop_Qt_5_13_0_MinGW_64_bit-Debug\\ki.jpg', 0)

#DCT变换
dct = cv2.dct(np.float32(img))

#设置低通滤波器
rows, cols = img.shape
crow,ccol = int(rows/2), int(cols/2) #中心位置
mask = np.zeros_like(dct, np.uint8)
mask[crow-30 : crow + 30, ccol-30 : ccol + 30] = 1

#掩膜图像和频谱图像乘积
f = dct * mask

# 转换为可以显示的图片（fshift_low），fshift_low中包含实部和虚部
spectrogram = np.log(np.abs(f))

#DCT逆变换
iimg = cv2.idct(f)

#显示原始图像和低通滤波处理图像
plt.subplot(131), plt.imshow(img, 'gray'), plt.title('Original Image')
plt.axis('off')
plt.subplot(132), plt.imshow(spectrogram, 'gray'), plt.title('Spectrogram Image')
plt.axis('off')
plt.subplot(133), plt.imshow(iimg, 'gray'), plt.title('Result Image')
plt.axis('off')
plt.show()


