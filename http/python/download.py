#!/usr/bin/env python
# coding=utf-8

import os

# 处理对应参数

ev=os.environ
query=ev.get("QUERY_STRING")

# 发送正文
# 发送正文之前，先将客户端请求进行 Urlencode 解码
# 对文件名进行解码

from urllib import unquote 
query = unquote(query)

# 拼接路径
path = 'wwwroot/source/' + query

# 打开文件
fd = os.open(path, os.O_RDWR)
size = os.path.getsize(path)

# 发送响应报文
os.write(int(sock), 'HTTP/1.1 200 OK\r\n')

os.write(int(sock), 'Content-Type: application/octet-stream\r\n')
send_content = 'Content-Length: ' + str(size) + '\r\n'
os.write(int(sock), send_content)
send_buf = 'Content-Disposition: attachment;filename=' + query + '\r\n'
os.write(int(sock), send_buf)

# 发送空行
os.write(int(sock), '\r\n')

# 发送正文
cache = os.read(fd, size)
os.write(int(sock), cache)

# 关闭
os.close(fd)

