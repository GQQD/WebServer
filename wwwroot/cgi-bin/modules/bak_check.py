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

DIR_PROBE_EXTS = ['.tar.gz', '.zip', '.rar', '.tar.bz2']
FILE_PROBE_EXTS = ['.bak', '.swp', '.1']

def run(url):
    urlobj = urlparse.urlparse(url)
    paths = get_parent_paths(urlobj.path)
    web_paths = []
    for path in paths:
        if path == '/':
            #网站根目录
            for ext in DIR_PROBE_EXTS:
                url = "%s://%s%s%s" % (urlobj.scheme,urlobj.netloc,path,urlobj.netloc+ext)
                web_paths.append(url)
        else:
            if path[-1] == '/':
                #网站子目录
                for ext in DIR_PROBE_EXTS:
                    url = "%s://%s%s%s" % (urlobj.scheme,urlobj.netloc,path[:-1],ext)
                    web_paths.append(url)
            else:
                #非目录
                for ext in FILE_PROBE_EXTS:
                    url = "%s://%s%s%s"%(urlobj.scheme,urlobj.netloc,path,ext)
                    web_paths.append(url)
    downloader = Downloader.Downloader()
    found = False
    for path in web_paths:
        #print("[bak_check]:%s"%path)
        if((downloader.get(path) is not None)):
            found = True
            print("[bak_check]:bak found!%s"%path)
    if found is False:
        print ("[bak_check]:not found!")
