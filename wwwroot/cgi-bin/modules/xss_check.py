#!/usr/bin/python
#coding=utf-8
"""
*文件说明:xss_check.py
*作者:高小调
*创建时间:2017年08月13日 星期日 09时45分42秒
*开发环境:Kali Linux/Python v2.7.13
"""
import sys
import os
#初始化操作,将xss注入语句加载至内存
filename = os.path.join(sys.path[0],'data','xss.txt')
f = open(filename)  #这里可能存在异常
xss_code=[]
for code in f:
    print code
    xss_code.append(code)

