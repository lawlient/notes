# Windows开机启动WSL2（Ubuntu）的启动项

1. wsl中创建文件保存内容如下：
```
#! /bin/sh
service docker start
```

2. 添加执行权限`sudo chmod +x /etc/init.wsl`

3. windowns中创建文件wsl-start.vbs，内容如下：
```
Set ws = WScript.CreateObject("WScript.Shell")
ws.run "wsl -d ubuntu -u root /etc/init.wsl"
```

4.  `win+R`输入`shell:startup`保存刚才的文件至此路径


