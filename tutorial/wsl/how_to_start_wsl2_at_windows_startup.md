# Windows开机启动WSL2（Ubuntu）的启动项

1. wsl中创建文件/etc/init.wsl保存内容如下，自动ssh服务
```
#! /bin/sh

# 启动ssh服务
sudo service ssh start

# 启动cron服务
sudo service cron start

# 将wsl ip地址刷新至windows hosts文件，需要hosts文件中存在xxx wsl
sudo sed -i "s/^.* wsl/$(ifconfig eth0 | grep 'inet ' | awk '{print $2}') wsl/" /mnt/c/Windows/System32/drivers/etc/hosts
```

2. 添加执行权限`sudo chmod +x /etc/init.wsl`

3. windowns中创建文件wsl-start.vbs，内容如下：
```
Set ws = WScript.CreateObject("WScript.Shell")
ws.run "wsl -d $your-wsl-release-version -u root /etc/init.wsl"
```

4.  `win+R`输入`shell:startup`保存刚才的文件至此路径


