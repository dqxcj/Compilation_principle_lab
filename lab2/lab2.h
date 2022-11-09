#ifndef LAB2_H
#define LAB2_H
#include <unordered_map>
#include <vector>

const int IDENFR = 1; //标识符
const int INTCON = 2; //整数
const int CHARCON = 3; //字符
const int STRCON = 4; //字符串
const int CONSTTK = 5; //const
const int INTTK = 6; //int
const int CHARTK = 7; //char
const int VOIDTK = 8; //void
const int MAINTK = 9; //main
const int IFTK = 10; //if
const int ELSETK = 11; //else
const int DOTK = 12; //do
const int WHILETK = 13; //while
const int FORTK = 14; //for
const int SCANFTK = 15; //scanf 
const int PRINTFTK = 16; //printf
const int RETURNTK = 17; //return 
const int PLUS = 18; //+
const int MINU = 19; //-
const int MULT = 20; //*
const int DIV = 21; // /
const int LSS = 22; //<
const int LEQ = 23; //<=
const int GRE = 24; //>
const int GEQ = 25; //>=
const int EQL = 26; //==
const int NEQ = 27; //!=
const int ASSIGN = 28; //=
const int SEMICN = 29; //;
const int COMMA = 30; //,
const int LPARENT = 31; //(
const int RPARENT = 32; //)
const int LBRACK = 33; //[
const int RBRACK = 34; //]
const int LBRACE = 35; //{
const int RBRACE = 36; //}

// const int ADDGR = 41; //加法运算符
// const int MULGR = 42; //乘法运算符
// const int RELAGR = 43; //关系运算符
// const int ALPHGR = 44; //字母
// const int NUMGR = 45; //数字
// const int NZNUMGR = 46; //非0数字
// const int CHARGR = 47; //字符
// const int STRGR = 48; //字符串
const int PROCGR = 49; //程序
const int TFUNGR = 5960; //辅助程序 {有返回值函数定义|无返回值函数定义}
const int CSGR = 50; //常量说明
const int CSGRH = 501; //辅助常量说明
const int CDGR = 51; //常量定义
const int CDGRH1 = 511; //辅助常量定义1
const int CDGRH2 = 512; //辅助常量定义2
const int UINTGR = 52; // 无符号整数就是lab1的整数
const int INTGR = 53; //整数
//const int IDENGR = 54; //标识符
const int STAHGR = 55; //声明头部
const int VARSGR = 56; //变量说明
const int VARSGRH = 561; //辅助变量说明
const int VARDGR = 57; //变量定义
const int VARDGRH = 571; //辅助变量定义
const int TYIDGR = 58; //类型标识符 ,不用输出这个
const int REFUNGR = 59; //有返回值函数定义
const int VOFUNGR = 60; //无返回值函数定义
const int COMSEGR = 61; //复合语句
const int PARAGR = 62; //参数表
const int MAINGR = 63; //主函数
const int EXPGR = 64; //表达式
const int EXPGRH = 641; //辅助表达式
const int XIGR = 65; //项
const int XIGRH = 651; //辅助项
const int TRAGR = 66; //因子
const int SENGR = 67; //语句
const int EQSENGR = 68; //赋值语句
const int IFSENGR = 69; //条件语句
const int IFGR = 70; //条件
const int IFGRH = 701; //辅助条件
const int LOOPGR = 71; //循环语句
const int WLENGR = 72; //步长
const int REFUNUGR = 73; //有返回值函数调用语句
const int VOFUNUGR = 74; //无返回值调用语句
const int VPALIGR = 75; //值参数表
const int SELIGR = 76; //语句列
const int READSEGR = 77; //读语句
const int READSEGRH = 771; //辅助读语句
const int WRISEGR = 78; //写语句
const int RETUSEGR = 79; //返回语句


const int MUSTGR = 80; //必须的
const int NOTMUGR = 81; //不是必须的



const int E = 100;
const int T = 101;
const int E1 = 102;
const int NU = 103;  //NU不要修改
const int F = 104;
const int T1 = 105;
const int Jia = 106;
const int Chen = 107;
const int I = 108;
const int Lkh = 109;
const int Rkh = 110;




#endif