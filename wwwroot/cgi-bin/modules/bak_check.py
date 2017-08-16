#!/usr/bin/python
#coding=utf-8
"""
*文件说明:bak_check.py
*作者:高小调
*创建时间:2017年08月16日 星期三 09时37分38秒
*开发环境:Kali Linux/Python v2.7.13
"""
import sys
sys.path.append("..")
from lib import Downloader
import urlparse
def get_parent_paths(path):
    paths = []
    if not path or path[0] != '/':
        #path为None 或 path为/
        return paths
    paths.append(path)
    tmp = path
    if path[-1] == '/':
        #path 为 /a/b/c/d/e/
        tmp = path[:-1] #tmp = /a/b/c/d/e

    while tmp:  #tmp = /a/b/c/d/e
        tmp = tmp[:tmp.rfind('/')+1]   #tmp = /a/b/c/d/
        paths.append(tmp);
        tmp = tmp[:-1]                      #tmp = /a/b/c/d
    return paths

