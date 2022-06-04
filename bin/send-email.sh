#!/usr/bin/env bash

sender="jiuanx2011@163.com"                     #邮件发送者
stmp_addr="smtp.163.com"                        #服务器地址
topic="Monitor for diary"   		        #邮件主题
receiver="jiuanx2011@163.com"                   #邮件接收地址，使用逗号分隔

#邮件内容
msg="
你好！\n
    我是测试邮件。\n
祝你一切顺利！\n

Date: 2022-05-11\n
Addr: 702\n
"

CURRENT_DIR=$(cd `dirname $0`; pwd)
mail_name=`date +"%Y-%m-%d"`
mail_file_addr=$CURRENT_DIR/mails/$mail_name

cat /dev/null > $mail_file_addr
echo "Hello lawlient ! " >> $mail_file_addr
echo "" >> $mail_file_addr
echo "    Did you update your diary today ??? " >> $mail_file_addr
echo "" >> $mail_file_addr
echo "------------------------" >> $mail_file_addr
echo "Date: ` date  +\"%Y-%m-%d %H:%M:%S\"`" >> $mail_file_addr
echo "Addr: dell in 702" >> $mail_file_addr

psd=`cat /etc/lawlient/psd`
sendEmail -u $topic -f $sender -s $stmp_addr -t $receiver -xu $sender -xp $psd -o tls=auto message-file="$mail_file_addr"

rm $mail_file_addr

