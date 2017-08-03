#!/usr/bin/python
#coding=utf-8
"""
*文件说明:wwwroot/cgi-bin/python.py
*作者:高小调
*创建时间:2017年08月03日 星期四 15时38分03秒
*开发环境:Kali Linux/Python v2.7.13
"""

#!/usr/bin/python
# -*- coding: UTF-8 -*-

# CGI处理模块
import cgi, cgitb 

# 创建 FieldStorage 的实例化
form = cgi.FieldStorage() 

# 获取数据
site_name = form.getvalue('name')
site_url  = form.getvalue('url')

print "<html>"
print "<head>"
print "<meta charset=\"utf-8\">"
print "<title>菜鸟教程 CGI 测试实例</title>"
print "</head>"
print "<body>"
print "<h2>%s官网：%s</h2>" % (site_name, site_url)
print "</body>"
print "</html>"
