# Windows开机启动WSL2（Ubuntu）的启动项

1. wsl中创建文件/etc/init.wsl保存内容如下，自动ssh服务
```
#! /bin/sh
sudo service ssh start
```

2. 添加执行权限`sudo chmod +x /etc/init.wsl`

3. windowns中创建文件wsl-start.vbs，内容如下：
```
Set ws = WScript.CreateObject("WScript.Shell")
ws.run "wsl -d $your-wsl-release-version -u root /etc/init.wsl"
```

4.  `win+R`输入`shell:startup`保存刚才的文件至此路径


