import cv2
import numpy as np
from PIL import Image

# 设置参数
width, height = 426,694
fps = 24
num_frames = 100

# 方块参数
block_w, block_h = 100, 100
top_color = (0, 200, 255)   # 
bottom_color = (0, 200, 255) # 

frames = []

for i in range(num_frames):
    # 创建空白背景
    frame = np.zeros((height, width, 3), dtype=np.uint8)
    
    # 上方方块位置（上下运动示例）
    top_y = int((height/4 - block_h/2) + 50*np.sin(2*np.pi*i/num_frames))
    frame[top_y:top_y+block_h, width//4:width//4+block_w] = top_color

    # 下方方块位置（上下运动示例）
    bottom_y = int((3*height/4 - block_h/2) + 50*np.sin(2*np.pi*i/num_frames + np.pi))
    frame[bottom_y:bottom_y+block_h, width//4:width//4+block_w] = bottom_color

    # 转为 PIL Image 添加到帧列表
    frames.append(Image.fromarray(frame))

# 保存为 GIF
frames[0].save('two_blocks.gif', save_all=True, append_images=frames[1:], duration=int(1000/fps), loop=0)
print("动图生成完成！")

