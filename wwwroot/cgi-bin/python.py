#!/usr/bin/python
#coding=utf-8
"""
*文件说明:wwwroot/cgi-bin/python.py
*作者:高小调
*创建时间:2017年08月03日 星期四 15时38分03秒
*开发环境:Kali Linux/Python v2.7.13
"""
#WebScan相关模块
from modules import cdn_check
from modules import port_scan
import requests
from sys import argv
from lib import Spider
from modules import sql_check

# CGI处理模块
import cgi, cgitb
# 创建 FieldStorage 的实例化
form = cgi.FieldStorage() 
# 获取数据
url  = form.getvalue('url')
email = form.getvalue("email")
print "<html>"
print "<head>"
print "<meta charset=\"utf-8\">"
print "<title>菜鸟教程 CGI 测试实例</title>"
print '<script src="https://cdn.bootcss.com/jquery/1.12.4/jquery.min.js"></script>'
print '<script src="http://blog.gaoxiaodiao.com/style/js/gxd.js"></script>'
print "</head>"
print "<body>"
if url is None or email is None:
    print "<p>请输入要检测的url!</p>"
else:
    print "<p>正在检测中.</p>"
    print '<script>alert("检测完成后,会自动将结果发送至您的邮箱!");$.get("http://localhost/cgi-bin/webscan.py?url='+url+'&email='+email+'");history.go(-1);</script>'
print "</body>"
print "</html>"
