#!/usr/bin/python
#coding=utf-8
import smtplib
from email.header import Header
from email.mime.text import MIMEText

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
