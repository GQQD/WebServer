#!/usr/bin/python
#coding=utf-8
"""
*文件说明:webcms_check.py
*作者:高小调
*创建时间:2017年08月03日 星期四 14时22分08秒
*开发环境:Kali Linux/Python v2.7.13
"""
import sys,os,Queue,threading,hashlib,json
sys.path.append("..")
from lib import Downloader
import chardet
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
        if(self.work_queue.empty() and self.NotFound is not False):
            self.NotFound = False
            #print "[webcms_check] Not Found!"
            print("未知")
            return False 
        if(self.NotFound is False):
            return False
        cms = self.work_queue.get()
        _url = self.url + cms["url"]
        html = self.Downloader.get(_url)
        print "[webcms_check]:checking %s"%_url
        tmp = cms["re"].encode('utf-8')
        if(html is None):
            return False
        if tmp :
            if(html.find(tmp) != -1):
                self.result = cms["name"]
                self.NotFound = False
                #print "[webcms_check]cms is"%(cms["name"])
                print(cms["name"])
                return True
        else:
            md5 = self.md5(html)
            if(md5 == cms["md5"]):
                self.result = cms["name"]
                self.NotFound = False
                print(cms["name"])
                return True

    def run(self):
        while(self.NotFound):
            th = []
            for i in range(self.thread_num):
                t = threading.Thread(target=self.thread_what_web)
                t.start()
                th.append(t)
            for t in th:
                t.join()
            if(self.result):
                print(cms["name"])
                #此处可以进行输
