# 如何构建基于brpc框架的服务


## 构建brpc基础镜像

基于[Dockerfile](Dockerfile)构建基础镜像。brpc仓库下的Dockerfile文件并未安装，本文为修改后的版本。

```
docker build -f Dockerfile -t brpc .
```


## 支持mysqlcppconn

通过官方地址[here](https://dev.mysql.com/downloads/connector/cpp/)下载对应版本的库并完成安装。


## demo

[demo](demo/clent.cpp)


### build

```
cd demo
mkdir build
cd build
cmake .. 
make
```


