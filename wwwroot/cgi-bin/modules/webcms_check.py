#!/usr/bin/python
#coding=utf-8
"""
*文件说明:webcms_check.py
*作者:高小调
*创建时间:2017年08月03日 星期四 14时22分08秒
*开发环境:Kali Linux/Python v2.7.13
"""
import sys,Queue,threading,os,hashlib
sys.path.append("..")
from lib import Downloader

class webcms(object):
    work_queue = Queue.Queue()
    url = ""
    thread_num = 0
    NotFound = True
    Downloader = Downloader.Downloader()
    result = ""
    def __init__(self,url,thread_num = 10):
        self.url = url
        self.thread_num = thread_num
        filename = os.path.join(sys.path[0],"data","data.json")
        fp = open(filename)
        webdata = json.load(fp,encoding="utf-8")
        for i in webdata:
            self.work_queue.put(i)
        fp.close()
    
    def md5(self,body):
        m = hashlib.md5()
        m.update(body)
        return m.hexdigest()
    
    def thread_what_web(self):
        if(self.work_queue.empty()):
            self.NotFound = False
        return False
        if(self.NotFound is False):
            cms = self.work_queue.get()
            _url = self.url + cms["url"]
            html = self.Downloader.get(_url)

