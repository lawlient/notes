**ubuntu 升级软件**
```
sudo apt-get update 更新源
sudo apt-get upgrade 更新已安装的包
sudo apt-get dist-upgrade 升级系统
```


**ubuntu升级特定软件**
`sudo apt-get install packgname`


**查看软件是否安装**
`dpkg -l | grep filename`


**查看安装路径**
`dpkg -L 软件名`

**附录**
```
apt-cache search package 搜索包
apt-cache show package 获取包的相关信息，如说明、大小、版本等
sudo apt-get install package 安装包
sudo apt-get install package - - reinstall 重新安装包
sudo apt-get -f install 修复安装"-f = ——fix-missing"
sudo apt-get remove package 删除包
sudo apt-get remove package - - purge 删除包，包括删除配置文件等
sudo apt-get update 更新源
sudo apt-get upgrade 更新已安装的包
sudo apt-get dist-upgrade 升级系统
sudo apt-get dselect-upgrade 使用 dselect 升级
apt-cache depends package 了解使用依赖
apt-cache rdepends package 是查看该包被哪些包依赖
sudo apt-get build-dep package 安装相关的编译环境
apt-get source package 下载该包的源代码
sudo apt-get clean && sudo apt-get autoclean 清理无用的包
sudo apt-get check 检查是否有损坏的依赖
sudo apt list --upgradable
```
|dir|function|
|---|--------|
|bin|         用以存储二进制可执行命令文件，/usr/bin/也存储了一些基于用户的命令文件|
|sbin|        许多系统命令的存储位置，/usr/sbin/中也包括了许多命令|
|root|        超级用户，即根用户的主目录|
|home|        普通用户的默认目录，在该目录下，每个用户拥有一个以用户名命名的文件夹|
|boot|        存放Ubuntu内核和系统启动文件|
|mnt|         通常包括系统引导后被挂载的文件系统的挂载点|
|dev|         存储设备文件，包括计算机的所有外部设备，如硬盘、是、键盘、鼠标等|
|etc|         存放文件管理配置文件和目录|
|lib|         存储各种程序所需要的共享库文件|
|lost+found|  一般为空，当非法关机时，会存放一些零散的文件|
|var|         用于存放很多不断变化的文件，例如日志文件等|
|usr|         包括与系统用户直接有关的文件和目录|
|media|       存放Ubuntu系统自动挂载的设备文件|
|proc|        这是一个虚拟目录，它是内存的映射，包括系统信息和进程信息|
|tmp|         存储系统和用户的临时信息|
|initrd|      用来加载启动时临时挂载的initrd.img映像文件，以及载入所要的设备模块目录|
|opt|         作为可选文件和程序的存放目录，否则将无法引导计算机进入操作系统|
|srv|         存储系统提供的服务数据|
|sys|         系统设备和文件层次结构，并向用户程序提供详细的内核数据信息|

------
**vim can not present chinese font on ubuntu**

首先，添加中文字符编码： 
`sudo vim /var/lib/locales/supported.d/local`

在打开的文件中添加下面的中文字符集
```
zh_CN.GBK GBK 
zh_CN.GB2312 GB2312 
zh_CN.GB18030 GB18030 
```

然后输入以下命令使其生效：
`sudo dpkg-reconfigure locales`

接下来是修改 vim 的配置文件
`sudo vim /etc/vim/vimrc`

在后面加入
```
set fileencodings=utf-8,gb2312,gbk,gb18030 
set termencoding=utf-8 
set encoding=prc 
```
保存后，重新打开 vim 就可以正常显示中文了。

--------

**upgrade gcc and g++ on ubuntu**

```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt list --upgradable // check software package can be upgraded
sudo apt install gcc-6 g++-6
```
----

**electronic-wechat on ubuntu**

```
sudo apt install snapd snapd-xdg-open
sudo snap install electronic-wechat
electronic-wechat // run the application
```

-----
**fonts install**

`sudo apt install ttf-mscorefonts-installer`

-----
**git diff ignore ^M**

`git config --global core.whitespace cr-at-eol`

----





