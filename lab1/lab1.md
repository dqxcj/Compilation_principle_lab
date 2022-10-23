# 实验一 词法分析（2021）
## 前言

注：词法分析和语法分析实验都针对此文法，语法分析实验需在词法分析实验的基础上进行增量式开发。

**【文法定义】:**

＜标识符＞::=＜字母＞｛＜字母＞｜＜数字＞｝    

＜字母＞::=＿｜a｜．．．｜z｜A｜．．．｜Z  

＜数字＞::=０｜１｜．．．｜９  

＜整数＞::=［＋｜－］＜无符号整数＞

＜无符号整数＞::=＜数字＞｛＜数字＞｝

＜字符＞::='＜加法运算符＞'｜'＜乘法运算符＞'｜'＜字母＞'｜'＜数字＞'  

＜加法运算符＞::=+｜- 

＜乘法运算符＞::=*｜/ 

＜字符串＞::="｛十进制编码为 32,33,35-126 的 ASCII 字符｝"         

**注意**: 
1. 标识符和关键字都不区分大小写，比如 if 和 IF 均为关键字，不允许出现与关键字相同的标识符      
2. 字符串中要求至少有一个字符  

![](https://raw.githubusercontent.com/dqxcj/Study/test/test2/test7/test8/202210232138880.png)

## 【问题描述】

请根据给定的文法设计并实现词法分析程序，从源程序中识别出单词，记录其单词类别和单词值，输入输出及处理要求如下：

   （1）数据结构和与语法分析程序的接口请自行定义；类别码需按下表格式统一定义；

   （2）为了方便进行自动评测，输入的被编译源文件统一命名为 testfile.txt（注意不要写错文件名）；输出的结果文件统一命名为 output.txt（注意不要写错文件名），结果文件中每行按如下方式组织：

单词类别码 单词的字符 / 字符串形式 (中间仅用一个空格间隔)

单词的类别码请统一按如下形式定义：

<table><tbody><tr><td><p>单词名称</p></td><td><p>类别码</p></td><td><p>单词名称</p></td><td><p>类别码</p></td><td><p>单词名称</p></td><td><p>类别码</p></td><td><p>单词名称</p></td><td><p>类别码</p></td></tr><tr><td><p>标识符</p></td><td><p>IDENFR</p></td><td><p>if</p></td><td><p>IFTK</p></td><td><p>-</p></td><td><p>MINU</p></td><td><p>=&nbsp;</p></td><td><p>ASSIGN</p></td></tr><tr><td><p>整数</p></td><td><p>INTCON</p></td><td><p>else</p></td><td><p>ELSETK</p></td><td><p>*</p></td><td><p>MULT</p></td><td><p>;</p></td><td><p>SEMICN</p></td></tr><tr><td><p>字符</p></td><td><p>CHARCON</p></td><td><p>do</p></td><td><p>DOTK</p></td><td><p>/</p></td><td><p>DIV</p></td><td><p>,</p></td><td><p>COMMA</p></td></tr><tr><td><p>字符串</p></td><td><p>STRCON</p></td><td><p>while</p></td><td><p>WHILETK</p></td><td><p>&lt;&nbsp;</p></td><td><p>LSS</p></td><td><p>(</p></td><td><p>LPARENT</p></td></tr><tr><td><p>const</p></td><td><p>CONSTTK</p></td><td><p>for</p></td><td><p>FORTK</p></td><td><p>&lt;=</p></td><td><p>LEQ</p></td><td><p>)</p></td><td><p>RPARENT</p></td></tr><tr><td><p>int</p></td><td><p>INTTK</p></td><td><p>scanf</p></td><td><p>SCANFTK</p></td><td><p>&gt;&nbsp;</p></td><td><p>GRE</p></td><td><p>[</p></td><td><p>LBRACK</p></td></tr><tr><td><p>char</p></td><td><p>CHARTK</p></td><td><p>printf</p></td><td><p>PRINTFTK</p></td><td><p>&gt;=</p></td><td><p>GEQ</p></td><td><p>]</p></td><td><p>RBRACK</p></td></tr><tr><td><p>void</p></td><td><p>VOIDTK</p></td><td><p>return</p></td><td><p>RETURNTK</p></td><td><p>==&nbsp;</p></td><td><p>EQL</p></td><td><p>{</p></td><td><p>LBRACE</p></td></tr><tr><td><p>main</p></td><td><p>MAINTK</p></td><td><p>+</p></td><td><p>PLUS</p></td><td><p>!=&nbsp;</p></td><td><p>NEQ</p></td><td><p>}</p></td><td><p>RBRACE</p></td></tr></tbody></table>

【输入形式】testfile.txt 中的符合文法要求的测试程序。  
【输出形式】要求将词法分析结果输出至 output.txt 中。

【特别提醒】

（1）读取的字符串要原样保留着便于输出，特别是数字，这里输出的并不是真正的单词值，其实是读入的字符串，单词值需另行记录。

（2）本次作业只考核对正确程序的处理，但需要为今后可能出现的错误情况预留接口。

（3）在今后的错误处理作业中，需要输出错误的行号，在词法分析的时候注意记录该信息。

（4）单词的类别和单词值以及其他关注的信息，在词法分析阶段获取后，后续的分析阶段会使用，请注意记录；当前要求的输出只是为了便于评测，完成编译器中无需出现这些信息，请设计为方便打开 / 关闭这些输出的方案。


[输入样例1](/lab1/testfile_01.txt)  
[样例输出1](/lab1/output_01.txt)  
[输入样例2](/lab1/testfile_02.txt)  
[样例输出2](/lab1/output_02.txt)  


[我的解答](/lab1/lab1.cpp)

