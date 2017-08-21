#!/usr/bin/python
#coding=utf-8
"""
*文件说明:scan.py
*作者:高小调
*创建时间:2017年08月16日 星期三 10时54分48秒
*开发环境:Kali Linux/Python v2.7.13
"""
import lib
import modules

def scan(url):
    modules.check_cdn.run(url)

scan("http://gaoxiaodiao.com")
