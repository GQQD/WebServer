#!/usr/bin/python
#coding=utf-8
"""
*文件说明:测试各种函数
*作者:高小调
*创建时间:2017年07月27日 星期四 16时15分18秒
*开发环境:Kali Linux/Python v2.7.13
"""
from modules import cdn_check
from modules import port_scan
import requests
from sys import argv
from lib import Spider
from modules import sql_check
from modules import webcms_check
from lib import Downloader
from modules import webshell_check
from modules import xss_check
from modules import bak_check
import socket
import urlparse
#测试检查页面是否存在sql注入
def test_sql_check():
    #ret = sql_check.run("http://www.hbxffy1.com/info/dispnews.asp?id=1709")
    #ret = sql_check.run("http://www.szxcc.com/gb/about1_xinxi.asp?id=325")
    ret = sql_check.run("http://www.szxcc.com/gb/zxgg_detail.asp?id=376")
    print(ret);
    #ret = sql_check.run("http://www.chinaxinge.com/company/skin/12/index.asp?id=7798")
    #print ret;

def FullScan(url):
    #对url进行格式化
    url_info = urlparse.urlparse(url)
    scheme = url_info.scheme
    netloc = url_info.netloc
    path = url_info.path
    query = url_info.query
    if scheme == "":
        scheme = 'http'
    if netloc == "":
        print("网站地址不合法:域名为空!")
        return
    url = scheme + "://" + netloc
    #先进行CDN检测
    cdn_exist = cdn_check.run(url)
    cdn = "不存在CDN"
    if cdn_exist is True:
        #不存在CDN,则进行端口扫描
        ip = socket.gethostbyname(netloc)
        port_scan.PortScan(ip)
    else:
        cdn = "存在CDN"
    #爬虫抓取网站动态页面
    spider = Spider.Spider(url,10)
    dynamic_urls = spider.craw()
    sql="存在sql漏洞"
    xss="存在xss漏洞"
    sql_urls = []
    xss_urls = []
    for dynamic_url in dynamic_urls:
        if sql_check.run(dynamic_url) is True:
            sql_urls.append(dynamic_url)
        if xss_check.run(url) is True:
            xss_urls.append(dynamic_url)
    if sql_urls is None:
        sql = "无sql漏洞"
    if xss_urls is None:
        xss = "无xss漏洞"
    #CMS指纹识别
    cms_check = webcms_check.webcms(url,10)
    cms = cms_check.run()
    print("CDN:"+cdn)
    print("sql:"+sql)
    print("xss:"+xss)
    print("webcms:"+cms)
def SqlScan(url):
    #对url进行格式化
    url_info = urlparse.urlparse(url)
    scheme = url_info.scheme
    netloc = url_info.netloc
    path = url_info.path
    query = url_info.query
    if scheme == "":
        scheme = 'http'
    if netloc == "":
        print("网站地址不合法:域名为空!")
        return
    url = scheme + "://" + netloc + path
    spider = Spider.Spider(url,10)
    dynamic_urls = spider.craw()
    sql_urls = []
    for dynamic_url in dynamic_urls:
        if sql_check.run(dynamic_url) is True:
            sql_urls.append(dynamic_url)
    if len(sql_urls)==0:
        print("不存在sql漏洞")
    else:
        print("以下url存在sql漏洞:%d"%len(sql_urls))
        for sql_url in sql_urls:
            print(sql_url)
if __name__ == '__main__':
    if argv[1] == "0":
        FullScan(str(argv[2]))
    else:
        SqlScan(str(argv[2]))
