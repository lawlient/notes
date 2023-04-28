# HTMT

超文本标记语言（英语：HyperText Markup Language，简称：HTML）是一种用于创建网页的标准标记语言。



##  head 设置页面头部信息，不展示，全局有效

- title 设置标题
- base 单标签，href和\_target 2个属性，设置绝对地址
- meta 单标签，用户不可见，定义页面信息的名称、关键字、作者等，可多个meta标签




## body 页面主体


- p 段落开始
- hr 单标签，设置分割线
- nobr 强制不换行
- br 强制换行
- font 设置文字样式
- b 文字加粗
- u 文字下划线
- 特殊符号，&emsp 空格，&copy; 版权符号等
- plaintext，单标签，其后内容不做html解析
- xmp，双标签，其中内容标签失效
- sup上标格式，如指数的幂
- sub下标格式，如底数
- strike删除线
- marquee 包含内容文字滚动
- a 锚实现页面内部快速跳转
- mailto 超链接中设置电子邮件地址
- ftp超链接中设置ftp服务器链接
- telnet设置服务器链接

### 图片

- BMP 微软的点位图格式
- JPEG 平台无关格式，支持最高级别压缩，存在损耗
- GIF 8-256色图片，支持透明度、压缩、交错、多图像（动画）
- PNG 平台无关格式，支持透明度、压缩



### 外部资源
- bgsound 单标签、设置背景音乐，midi、wav、mp3
- embed 双标签，
- script插入js脚本


### 表格

- table
- th
- tr
- td

传统网页开发中通过表格对页面进行布局。

### 表单

- form
- input 单行文本框
- textarea 多行文本框
- button ，多标签，包含文本为按钮显示内容，用于JavaScript实现动态行为
- input 按钮，3种type:
    + button 普通按钮
    + reset 复位按钮
    + submit 提价按钮
    + radio 单选框
    + checkbox 复选框
- select 下拉列表
    + option 数据选项
- fieldset 表单功能分组
    + legend 分组标题


## 框架集页面

通过帧组织多个页面，可以在单个浏览器页面同时进行展示

- frameset 帧结构，组织不同的html页面，必须使用cols和rows属性
    + frame 单个帧页面

- iframe 比frameset更灵活，可嵌入html页面
