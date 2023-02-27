1. reinstall openssh service

```
sudo apt purge openssh-server

sudo apt install openssh-server
```

2. change config file

```
Port *yourport*
PasswordAuthentification yes
```

3. restart ssh service
```bash
sudo service ssh restart
```

4. set port proxy in powershell with admin
```
netsh interface portproxy set v4tov4 listenport=*yourport* listenaddress=0.0.0.0 connectport=*wsl port* connectaddress=*wsl ip*
```

5. show all port proxy config
```
netsh interface portproxy show all
```

6. setting firewall in windows 

- windows安全中心-》防火墙和网络保护-》高级设置-》入站规则-》新建规则

```
1.规则类型：端口
2.协议和端口：TCP，特定本地端口：2233
3.操作：允许连接；
4.配置文件：全选；
5.名称：任意；
```

6. test from remote side with ssh
