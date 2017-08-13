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
sys.path.append("..")
from lib import Downloader
#初始化操作,将xss注入语句加载至内存
filename = os.path.join(sys.path[0],'data','xss.txt')
f = open(filename)  #这里可能存在异常
xss_code=[]
for code in f:
    #print code
    xss_code.append(code)

#将url中的每个参数信息替换为xss_code,
def urlsplit(url):
    domain = url.split("?")[0]  #取出?之前的东西
    _url = url.split("?")[1]    #获取参数部分
    pararm = {}
    #将url中参数名与参数值放入pararm字典中
    for val in _url.split("&"):
        pararm[val.split("=")[0]] = val.split("=")[1]
    
    urls = []
    for val in pararm.values():
        new_url = domain + _url.replace(val,'xss_code')
        urls.append(new_url)
    return urls

def run(url):
    print "xss_check is running..." 
