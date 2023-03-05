# 搭建监控系统


## 背景

通过对个人每日24小时的使用进行简单统计分析，旨在提高时间利用效率，减少无意义时间投入占比。

时间分类，按照大类分成 **6** 类：

1. Investment
2. Sleep
3. Blackhole
4. Relax
5. Trival
6. Other

## 方案设计


### 目标

实现可视化图表展示，主要2个维度：

1. 一段时间（如周、月、季度、年）的整体情况
2. 不同类型时间随时间线的变化趋势


### 主要问题

数据可视化的问题基本可以归结为如下几点：

1. 原始数据采集
2. 数据存储
3. 数据分析与统计
4. 数据展示

### 架构设计

1. 个人日记系统中完成时间分类登记（这部分目前为手动统计，除非实现对大脑的自动监控）
2. mysql实现数据的结构化存储，关系型数据库，成熟
3. grafana 在数据可视化方面做的十分优秀，符合个人需求

架构图如下：

![架构交互图]()



## 实现

由于mysql、 grafana都是成熟系统，感叹**docker**虚拟化对服务部署做出的杰出贡献，实现简单的系统搭建实在方便。

### 1. 安装mysql

```docker pull mysql```拉取mysql镜像

```docker run -itd --name mysql -p 3306:3306 -e MYSQL_ROOT_PASSWORD=123456 mysql```启动mysql容器

建表语句
```
create table `time_table` (
    `id` INT NOT NULL AUTO_INCREMENT,
    `day` DATE,
    `i` DOUBLE,
    `s` DOUBLE,
    `r` DOUBLE,
    `b` DOUBLE,
    `t` DOUBLE,
    `update_time` DATETIME,
    primary key(`id`)
)ENGINE=InnoDB DEFAULT charset=utf8;
```


### 2. grafana安装


```docker pull grafana/grafana``` 拉取grafana镜像

```docker run --name grafana -d -p 3000:3000 grafana/grafana```启动grafana服务容器


### 3. grafana配置

#### 登录

- 浏器打开: `localhost:3000`
- 登录页面，初始用户名、密码：admin
- 点击登录
- 修改密码


#### 创建dashboard

1. Click the New dashboard item under the Dashboards icon in the side menu.
2. On the dashboard, click Add a new panel.
3. In the New dashboard/Edit panel view, go to the Query tab.
4. Configure your query by selecting -- Grafana -- from the data source selector. This generates the Random Walk dashboard.
5. Click the Save icon in the top right corner of your screen to save the dashboard.
6. Add a descriptive name, and then click Save.


#### 添加数据源

1. Configuration > Data Source >  Add New data source > 选择mysql
2. 配置Host、DataBase、User、Password
3. Save & test


#### 添加panel


数据采集语句


## 参考文档

- [Build your first dashboard](https://grafana.com/docs/grafana/latest/getting-started/build-first-dashboard/)
