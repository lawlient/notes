#!/usr/bin/env bash

sender="jiuanx2011@163.com"                     #邮件发送者
stmp_addr="smtp.163.com"                        #服务器地址
topic="test send email script from xujiuan"     #邮件主题
receiver="jiuanx2011@163.com"                   #邮件接收地址，使用逗号分隔

#邮件内容
msg="
你好！\n
    我是测试邮件。\n
祝你一切顺利！\n

Date: 2022-05-11\n
Addr: 702\n
"

sendEmail -u $topic -f $sender -s $stmp_addr -t $receiver -xu $sender -xp $1 -o tls=auto message-file="./mail"

