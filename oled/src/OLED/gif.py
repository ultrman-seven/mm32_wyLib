# -*- coding: utf-8 -*-
# from io import TextIOWrapper
from unittest import result
from PIL import Image
from PIL import ImageSequence
import numpy as np

def pic2bits(pic:Image,file, col=128, line =64):
    result = ""
    for page in range(int(line/8)):
        for c in range(col):
            data=0
            for l in np.arange(page*8+8,page*8,-1): #range(page*8,page*8+8):
                # data += int(pic.getpixel((l, c))/255)
                # data += int(pic.getpixel((c, l))/255)
                data <<=1
                if pic.getpixel((c, int(l-1))) > 0:
                    data += 1
            result+=(hex(data))+","
            # file.write(hex(data))
    file.write(result[:-1])
    # return result

img = Image.open("mao.gif")
i = 0
NAME = 'qe_gif'
start = f'''#include"common.h"
extern const uint8_t {NAME}[][1024];
const uint8_t {NAME}[][1024] = {{
'''
end='''
};
'''
with open(f'./pics/{NAME}.cpp','w') as qe:
    qe.write(start)
    for frame in ImageSequence.Iterator(img):
        f = frame.convert('1')
        f = f.resize((128, 64), Image.ANTIALIAS)
        qe.write('{')
        pic2bits(f,qe)
        qe.write('},\n')
        f.save("./pics/mao%d.png" % i)
        i+=1
    qe.write(end)
