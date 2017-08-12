#!/usr/bin/python
#coding=utf-8
"""
*文件说明:webshell_check.py
*作者:高小调
*创建时间:2017年08月12日 星期六 18时31分26秒
*开发环境:Kali Linux/Python v2.7.13
"""
import os
import sys
sys.path.append("..")
from lib import Downloader

#加载数据字典到内存中
filename = os.path.join(sys.path[0],'data','web_shell.dic')
#print filename
passwds = []
fp = open(filename)
for i in fp:
    passwds.append(i.strip())
def run(url):
    if(not url.endswith(".php")):
        return False
    print '[Webshell_check]:%s'%url
    post_data = {}
    for passwd in passwds:
       #some request 
        # post_data[passwd] = 'echo "password is %s";'%passwd
        downloader = Downloader.Downloader()
        ret = downloader.post(url,post_data)
        if(ret):
            print "webshell:%s"%ret
            return True
    return False
