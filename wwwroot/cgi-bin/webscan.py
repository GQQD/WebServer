#!/usr/bin/python
#coding=utf-8

import cgi,cgitb
import time
from modules import cdn_check
from modules import port_scan
import requests
from sys import argv
from lib import Spider
from modules import sql_check
from modules import send_email

#从获取cgi参数
form = cgi.FieldStorage()
url = form.getvalue("url")
email = form.getvalue("email")

#执行扫描任务
#cdn_msg,state = cdn_check.run(url)

#组织email内容
msg = "Your url is:" + url + "\n"
msg += "Your email is:" + email + "\n"
msg += "CDN:" + cdn_msg + "\n"

#发送email
handle = send_email.Email()
handle.login()
handle.send("805888196@qq.com",msg)
