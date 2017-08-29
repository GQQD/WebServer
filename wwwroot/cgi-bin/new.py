#!/usr/bin/python
#coding=utf-8
import sys  
import re   
import urllib2  
from bs4 import BeautifulSoup  
def search(key,limit=20):
    page = 1
    while page*10 <= limit:
        if page == 1:
            search_url='http://www.baidu.com/s?wd=key&rsv_bp=0&rsv_spt=3&rsv_n=2&inputT=6391'
        else:
            search_url='http://www.baidu.com/s?wd=key&rsv_bp=0&rsv_spt=3&rsv_n=2&inputT=6391&pn='+str((page-1)*10)
        req=urllib2.urlopen(search_url.replace('key',key))   
        html=req.read()
        soup=BeautifulSoup(html,"html.parser")
        linkpattern=re.compile("href=\"(.+?)\"")
        div=soup.find('div',id='wrapper').find('div',id='wrapper_wrapper').find('div',id='container').find('div',id='content_left')
        re_dict={}
        start = 1
        end = 10
        if page > 1:
            start = (page-1)*10+1
            end = page*10
        for i in range(start,end):
            tmp1 = div.find('div',id=str(i))
            if(tmp1 is None):
                print i
                continue
            tmp2 = tmp1.find('h3')
            a = tmp2.find('a')
            re_link=linkpattern.findall(str(a))
            re_title=a.text
            re_dict[re_title]=re_link[0]
        for r in re_dict:
            print r
            print re_dict[r]
            print '....................\n'
        page += 1
    return re_dict

if __name__=='__main__':  
        key=raw_input('input key word:')  
        search(key)
