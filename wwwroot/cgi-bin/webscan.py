#!/usr/bin/python
#coding=utf-8
import smtplib
from email.header import Header
from email.mime.text import MIMEText
import cgi,cgitb
import time
from modules import cdn_check
from modules import port_scan
import requests
from sys import argv
from lib import Spider
from modules import sql_check

class Email(object):
    def __init__(self,user="805888196@qq.com",passwd="cursfxypnffebeej"):
        self.user = user
        self.passwd = passwd
    def login(self):
        try:
            self.smtp = smtplib.SMTP_SSL("smtp.qq.com",465)
            self.smtp.login(self.user,self.passwd)
        except smtplib.SMTPException,e:
            print "Login Failed;%s"%e
    def send(self,receiver="805888196@qq.com",msg="Hello World\n"):
        msg = MIMEText(msg)
        msg["Subject"] = "------网站安全检测报告------"
        msg["From"] = self.user
        msg["To"] = receiver
        try:
            self.smtp.sendmail(self.user,receiver,msg.as_string())
        except smtplib.SMTPException,e:
            print "Send Failed;%s"%e
    def __del__(self):
        self.smtp.quit()

form = cgi.FieldStorage()
url = form.getvalue("url")
email = form.getvalue("email")
cdn_msg,state = cdn_check.run(url)
msg = "Your url is:" + url + "\n"
msg += "Your email is:" + email + "\n"
msg += "CDN:" + cdn_msg + "\n"

handle = Email()
handle.login()
handle.send("805888196@qq.com",msg)

