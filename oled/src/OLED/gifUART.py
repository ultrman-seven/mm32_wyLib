# -*- coding: utf-8 -*-
# from io import TextIOWrapper
from time import sleep
from unittest import result
from PIL import Image
from PIL import ImageSequence
import numpy as np
import serial

def pic2bits(pic:Image,file:serial.Serial, col=128, line =64):
    result = []
    for page in range(int(line/8)):
        for c in range(col):
            data=0
            for l in np.arange(page*8+8,page*8,-1): #range(page*8,page*8+8):
                # data += int(pic.getpixel((l, c))/255)
                # data += int(pic.getpixel((c, l))/255)
                data <<=1
                if pic.getpixel((c, int(l-1))) == 0:
                    data += 1
            result.append(data)
    file.write(bytearray(result))
    return result


def openCom(comName:str, bodeRate:int)->serial.Serial:
    com = serial.Serial(comName,bodeRate,timeout=0.5)
    if com.isOpen():
        print("serial open success")
        # data = com.readline()
        return com

img = Image.open("./gifs/q.gif")
mm32 = openCom('COM12',115200)
for i in range(5):
    for frame in ImageSequence.Iterator(img):
        f = frame.convert('1')
        f = f.resize((128, 64), Image.ANTIALIAS)
        pic2bits(f,mm32)
        sleep(0.05)
