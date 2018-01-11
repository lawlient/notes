#!/bin/bash
#倒计时脚本
#用法 scriptname time (格式为: ([0-9]+h)?([0-9]m)?([0-9]+s)? , 如 scriptname 1h1m32s 代表 1小时1分钟32秒)
#ubuntu 13.04 32 测试
#需安装 gnome-osd-client sudo apt-get install ..


#把时间转换为秒
function t2s
{
h=`echo $1 |egrep -o '[0-9]+h' |egrep -o '[0-9]+'`
m=`echo $1 |egrep -o '[0-9]+m' |egrep -o '[0-9]+'`
s=`echo $1 |egrep -o '[0-9]+s' |egrep -o '[0-9]+'`


if [ ! -z $h ]
then
h=`expr $h \* 3600`
else
h=0
fi

if [ ! -z $m ]
then
m=`expr $m \* 60`
else
m=0
fi

if [ -z $s ]
then
s=0
fi



echo `expr $h + $m + $s`


}
#把秒转换为时间
function s2t
{
s=$1
prs=`expr $s % 60`
m=`expr \( $s - $prs \) / 60`
prm=`expr $m % 60`
h=`expr \( $m - $prm \) / 60`

[ ${h} -lt 10 ] && h="0${h}"
[ ${prm} -lt 10 ] && prm="0${prm}"
[ ${prs} -lt 10 ] && prs="0${prs}"

echo ${h}h:${prm}m:${prs}s
}




counter=${1-1m}
counter=`t2s ${counter}`




while [ ${counter} -gt 0 ]
do
tm=`s2t ${counter}`
gnome-osd-client -f "<message id='test' osd_fake_translucent_bg='on' osd_vposition='top' osd_halignment='center'><span size='20000' foreground='red'>${tm}</span></message>"
sleep 1
((--counter))
done

tm=`s2t ${counter}`
gnome-osd-client -f "<message id='test' osd_fake_translucent_bg='on' osd_vposition='top' osd_halignment='center'><span size='20000' foreground='blue'>${tm}</span></message>"



#EOF
############################
