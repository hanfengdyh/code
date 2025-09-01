import cv2
import numpy as np
from PIL import Image
import math

# 设置参数
width, height = 694, 426   # 宽高
fps = 24
num_frames = 100

# 方块参数
block_w, block_h = 100, 100
left_color = (0, 200, 255)   
right_color = (0, 200, 255)  

frames = []

for i in range(num_frames):
    # 创建空白背景
    frame = np.zeros((height, width, 3), dtype=np.uint8)

    # 水平偏移，sin波控制来回运动
    offset = int((width/4 - block_w/2) * math.cos(2*math.pi*i/num_frames))

    # 左方块：从左边往中间
    left_x = int(width/4 - block_w/2 + offset)
    left_y = height//2 - block_h//2
    frame[left_y:left_y+block_h, left_x:left_x+block_w] = left_color

    # 右方块：从右边往中间
    right_x = int(3*width/4 - block_w/2 - offset)
    right_y = height//2 - block_h//2
    frame[right_y:right_y+block_h, right_x:right_x+block_w] = right_color

    # 转为 PIL Image 添加到帧列表
    frames.append(Image.fromarray(frame))

# 保存为 GIF
frames[0].save('two_blocks.gif', save_all=True, append_images=frames[1:], duration=int(1000/fps), loop=0)
print("动图生成完成！左右方块往中间靠拢并循环运动。")

