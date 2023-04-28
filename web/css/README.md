# CSS


CSS (Cascading Style Sheets，层叠样式表），是一种用来为结构化文档（如 HTML 文档或 XML 应用）添加样式（字体、间距和颜色等）的计算机语言，CSS 文件扩展名为 .css。


## 声明

1. 行间样式表。

```html
<p style="width:200px;height=300px; "> 内容</p>
```

特定场景下使用，表现与内容未分离，不建议使用。

2. 内部样式表。

```html
<style type=text/css>
    p{width:200px;
      height:300px;
     }
</style>
```

集中于头部信息的style标签中，使用广泛。

3. 外部样式表。

```html
<link rel="stylesheet" rev="stylesheet", href="index.css", type="text/css" />
```

即将css单独写入文件，通过head插入链接指定css文件。

表现与内容真正分离，推荐使用。


## 接口

- class
- id
- 选择符

```
选择符{
    属性1:值1;
    属性2:值2;
    属性n:值n;
}
```

css代码实际上是由选择符、属性、值组成的。


1. 标签选择符

如：
```css
p{ width:200px; }

h1{color:#ff0000; }
```

2. id 选择符

```css
#hello{color:#ff0000;}
```

id可自定义，唯一，不能数字开头


3. class 选择符

```css
.reader{ color:#ff0000; }
```

class名称可自定义，不能数字开头，


4. 伪类&伪对象选择符

预定义好的类和对象。不需要id和class声明。

```css
a:visited{color:#ff0000;}
```


5. 通配选择符

```css
*{margin:0px;}
```

所有对象的外边距为0像素。

---

选择符组合

1. 群组选择符

```css
p,span,div,li{color:#ff0000;}
```

2. 包含选择符

```css
p span{color:#ff0000;}
```

3. 标签指定式选择符

```css
p#hello{color:#ff0000;}
p.reader{color:#ff0000;}
```

4. 自由组合选择符

```css
p#hello h1{color:#ff0000;}
```


> id 和 class 的区别?

> 没有本质区别。同样的id名称在页面中只使用一次，同样的class名称可在页面中反复使用。  
> id 和 class 名称大小写敏感。  

|   单位        |   说明                |
|:-------------:|:----------------------|
| px            |   像素                |
| em            |   相对当前文本尺寸的倍数|
| ex            |   相对字符高度的倍数  |
| pt            |   点/磅               |
| pc            |   派卡                |
| in            |   英寸                |
| mm            |   毫米                |
| cm            |   厘米                |



## div

- 块状元素。硬矩形，具备高度和宽度。
- 内联元素。形式布袋，无法设置宽度和高度，由内容决定形状。


float属性--浮动。
- left 向左浮动
- right 向右浮动
- none 不浮动
- inherit 继承父容器属性

### 段落样式

对应p标签
- text-indent 首行缩进属性。常用2em，即本行2个文字距离。
- vertical-align 垂直对齐属性。
- line-height 调整行距
- letter-spacing 字符间距
- word-spacing 英文单词间距，对中文无效
- overflow 内容裁剪控制
- layout-flow 纵向、横向排版控制。如中文古文即纵向排版。
- clip属性对图片进行裁剪，直接调整宽高可能引起图片变形
- filter滤镜控制





