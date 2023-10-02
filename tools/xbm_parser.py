#!/bin/python3

import re
import struct
import os
"""
“lbm”格式定义：
扩展名.lbm
单色位图
小端
2字节宽度
2字节高度
4字节文件大小
之后是数据
"""
def writeLBM(filename: str, width: int, height: int, data: bytes):
    f = open(filename, "wb")
    f.write(struct.pack("<H", width))
    f.write(struct.pack("<H", height))
    f.write(data)
    f.close()

def parseXBM(filename: str):
    wid_pattern = "_width\s(\d+)"
    hei_pattern = "_height\s(\d+)"
    f = open(filename, "r")
    data = f.read()
    f.close()
    wid = re.findall(wid_pattern, data)
    hei = re.findall(hei_pattern, data)
    data_pattern = "0x(\w+)"
    data_raw = re.findall(data_pattern, data)
    binary = b""
    for i in data_raw:
        binary += bytes.fromhex(i)
    if len(binary) == int(wid[0]) * int(hei[0]) / 8:
        print("文件解析成功！")
    print(wid[0])
    print(hei[0])
    writeLBM(filename.replace(".xbm", ".lbm"), int(wid[0]), int(hei[0]), binary)

files = []
files_raw = os.listdir()
for file in files_raw:
    if file.endswith(".xbm"):
        files.append(file)
if(len(files) == 0):
    print("在当前目录没有找到任何.xbm文件！")
    exit()

for file in files:
    print(file)
i = input("是否转换上述全部文件[y/N]")
if i == "y":
    for file in files:
        print(file)
        parseXBM(file)
