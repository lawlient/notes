#!/bin/bash

# 基本工作原理

# PCL为惠普定义的一种格式语言。惠普打印机可以识别该语言，并进行
# 相应的打印操作。所以将pdf/ps文件转化为PCL格式后，传输给打印机，
# 打印机即可打印出该文件。我们可以通过raw协议将PCL文件传输给
# 打印机，端口号为9100。

# gs负责将pdf/ps文件转化为PCL文件
# nc负责将PCL文件传输给打印机


# ip=192.168.233.249
port=9100
output="%stdout"

while getopts "d:h" arg
do 
    case $arg in 
        d) ip=$OPTARG
            ;;
        h) echo "usage: hpprint.sh -d ip_address file_to_print" 
         exit 0
         ;;
        ?)  
            echo "unknown argument"
            ;;
    esac    
done    

input_file="${!#}"

if [ ! -e $input_file ] ; then
    echo "File not exist"
    echo "Abort print"
    exit 1
fi  



if [[ $ip =~ ^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$ ]]; then 
    gs  -q -dSAFER -dNOPAUSE -sDEVICE=ljet4d -sPAPERSIZE=a4 -dPDFFitPage -sOutputFile="$output" "$input_file" -c quit \
        | nc $ip $port
    echo "start to print"
    exit 0
else    
    echo "Illegal ip address"
    echo "Abort print"
    exit 1
fi

