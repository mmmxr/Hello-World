#!/usr/bin/env python
# coding=utf-8

import os

# 处理对应参数

ev=os.environ
query=ev.get("QUERY_STRING")
query=query[:-1]
# 发送正文
# 发送正文之前，先将客户端请求进行 Urlencode 解码
# 对文件名进行解码

from urllib import unquote 
query = unquote(query)
os.write(2, query)

# 拼接路径
path = 'wwwroot/source/' + query + '.zip'
os.write(2, path)

# 打开文件
fd = os.open(path, os.O_RDWR)
size = os.path.getsize(path)

# 发送响应报文
os.write(1, 'HTTP/1.1 200 OK\r\n')

os.write(1, 'Content-Type: application/octet-stream\r\n')
send_content = 'Content-Length: ' + str(size) + '\r\n'
os.write(1, send_content)
send_buf = 'Content-Disposition: attachment;filename=' + query + '.zip' + '\r\n'
os.write(1, send_buf)

# 发送空行
os.write(1, '\r\n')

# 发送正文
cache = os.read(fd, size)
os.write(1, cache)

# 关闭
os.close(fd)

