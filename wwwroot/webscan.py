#!/usr/bin/python
#coding=utf-8
"""
*文件说明:123.py
*作者:高小调
*创建时间:2017年08月05日 星期六 13时41分28秒
*开发环境:Kali Linux/Python v2.7.13
"""
import cgi,cgitb
form = cgi.FieldStorage()
url = form.getvalue("url")
email = form.getvalue("email")

html = open("/root/httpd/wwwroot/webscan.html","r")
try:
    output =  html.read()
finally:
    html.close()
print output;
html.close()
