# Ubuntu系统网络时钟同步


timedatectl命令

- 显示时区: `timedatectl list-timezones` 
- 设置时区为上海: `sudo timedatectl set-timezone Asia/Shanghai`。


## 修改时间为24小时制


打开/etc/default/locale文件，添加如下变量设置并重启机器即可。

`LC_TIME=en_DK.UTF-8`





