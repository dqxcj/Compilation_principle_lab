#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include "lab1.cpp"
#include "lab2Debug.cpp"
#include "lab2Struct.h"
using namespace std;

// pair.first是token的属性, pair.second是token本身
vector<pair<int, string>> tokens;
// index作为tokens的索引，指向当前处理的token
int index = 0;
ifstream in("lab1output.txt");
ofstream out("output.txt");

// 获取tokens
void GetTokens(ifstream &in, vector<pair<int, string>> &tokens);
// 输出单条token
void OutToken();
// 报错
void Error(const string &s);  
// <程序>
void IsProc();
// <常量说明>
void IsCs();
// <常量定义>
void IsCd();
// <无符号整数>
void IsUint();
// <整数>
void IsInt();
// <声明头部>
void IsStah();
// <变量说明>
void IsVars();
// <变量定义>
void IsVard();
// <有返回值函数定义>
void IsRefun();
// <无返回值函数定义>
void IsVofun();
// <复合语句>
void IsComse();
// <参数表>
void IsPara();
// <主函数>
void IsMain();
// <表达式>
void IsExp();
// <项>
void IsXi();
// <因子>
void IsTra();
// <语句>
void IsSen();
// <赋值语句>
void IsEqsen();
// <条件语句>
void IsIfsen();
// <条件>
void IsIf();
// <循环语句>
void IsLoop();
// <步长>
void IsWlen();
// <有返回值函数调用语句>
void IsRefunu();
// <无返回值函数调用语句>
void IsVofunu();
// <值参数表>
void IsVpali();
// <语句列>
void IsSeli();
// <读语句>
void IsReadse();
// <写语句>
void IsWrise();
// <返回语句>
void IsRetuse();

int main() {
    // 将源码转换为tokens并放入lab1output.txt
    Lab1();
    GetTokens(in, tokens);
    OutputTokens(tokens);
    IsProc();
    return 0;
}

void GetTokens(ifstream &in, vector<pair<int, string>> &tokens) {
    string line;
    while(getline(in, line)) {
        istringstream inFromStr(line);
        string word;
        vector<string> words;
        while(inFromStr >> word) {
            words.push_back(word);
        }
        if(inputRefer.find(words[0]) != inputRefer.cend()) {
            tokens.push_back({inputRefer[words[0]], words[1]});
        } else {
            cout << "error in function GetToken: inputRefer.find(words[0]) == inputRefer.cend()" << endl;
        }
    }
}

void IsProc() {
    // [常量说明]
    if(tokens[index].first == CONSTTK) {    
        IsCs();
    }
    // [变量说明] && 第一个<有返回值函数定义>
    if(tokens[index].first == INTTK || tokens[index].first == CHARTK) {   
        // 第一个<有返回值函数定义>， 其后两位必是'('
        if(tokens[index + 2].first == LPARENT) {
            ;
        } else {    //[变量说明]
            IsVars();
        }
    }
    // {＜有返回值函数定义＞|＜无返回值函数定义＞} && <主函数>
    while(tokens[index].first == INTTK || tokens[index].first == VOIDTK) {   
        // <主函数> && <无返回值函数定义>
        if(tokens[index].first == VOIDTK) {
            if (tokens[index + 1].first == MAINTK) {  //主函数
                IsMain();
                out << "<程序>" << endl;
                return;
            } else {  // <无返回值函数定义>
                IsVofun();
            }
        } else if(tokens[index].first == INTTK) {  // <有返回值函数定义>
            IsRefun();
        }
    }
    Error("IsProc");
}

void IsCs() {
    if(tokens[index].first != CONSTTK) {
        Error("IsCs");
    }
    OutToken(); // const
    index++;
    IsCd();
    OutToken(); // ';'
    index++;  
    while(tokens[index].first == CONSTTK) {
        OutToken(); // const
        index++;
        IsCd();
        OutToken(); // ';'
        index++;
    }
    cout << "<常量说明>" << endl;
}

void IsCd() {
    if(tokens[index].first == INTTK) {
        OutToken(); //int
        index++;
        OutToken(); //标识符
        index++;
        OutToken(); //'='
        index++;
        IsInt();
        while(tokens[index].first == COMMA) {
            OutToken(); // ','
            index++;
            OutToken(); // 标识符
            index++;
            OutToken(); // '='
            index++;
            IsInt();
        }
        cout << "<常量定义>" << endl;
    } else if(tokens[index].first == CHARTK) {
        OutToken(); //char
        index++;
        OutToken(); //标识符
        index++;
        OutToken(); //'='
        index++;
        OutToken(); //字符
        index++;
        while(tokens[index].first == COMMA) {
            OutToken(); // ','
            index++;
            OutToken(); // 标识符
            index++;
            OutToken(); // '='
            index++;
            OutToken(); // 字符
            index++;
        }
        cout << "<常量定义>" << endl;
    } else {
        Error("Iscd");
    }
}

void IsUint() {
    OutToken();
    index++;
    cout << "<无符号整数>" << endl;
}

void IsInt() {
    if(tokens[index].first == PLUS) {
        OutToken(); // '+'
        index++;
        IsUint();
        cout << "<整数>" << endl;
    } else if(tokens[index].first == MINU) {
        OutToken(); // '-'
        index++;
        IsUint();
        cout << "<整数>" << endl;
    } else if(tokens[index].first == INTCON) {
        IsUint();
        cout << "整数" << endl;
    } else {
        Error("IsInt");
    }
}

void IsStah() {
    if(tokens[index].first == INTTK) {
        OutToken(); //int
        index++;
        OutToken(); //标识符
        index++;
        cout << "<声明头部>" << endl;
    } else if(tokens[index].first == CHARTK) {
        OutToken(); // char
        index++;
        OutToken(); // 标识符
        index++;
        cout << "<声明头部>" << endl;
    } else {
        Error("IsStah");
    }
}

void IsVars() {
    if(tokens[index].first == INTTK || tokens[index].first == CHARTK) {
        IsVard();
        OutToken(); // ';'
        index++;
        while((tokens[index].first == INTTK || tokens[index].first == CHARTK) && tokens[index + 2].first != LPARENT) { // != '(' 排除调用IsVars()处有返回值函数定义的干扰
            IsVard();
            OutToken(); // ';'
            index++;
        }
        cout << "<变量说明>" << endl;
    } else {
        Error("IsVars");
    }
}

void IsVard() {
    if(tokens[index].first == INTTK || tokens[index].first == CHARTK) {
        OutToken(); // int | char
        index++;
        OutToken(); // 标识符
        index++;
        if(tokens[index].first == LBRACK) {
            OutToken(); // '['
            index++;
            IsUint();
            OutToken(); // ']'
            index++;
        }
        while(tokens[index].first == COMMA) {
            OutToken(); //','
            index++;
            OutToken(); // 标识符
            index++;
            if(tokens[index].first == LBRACK) {
                OutToken(); // '['
                index++;
                IsUint();
                OutToken(); // ']'
                index++;
            }
        }
        cout << "<变量定义>" << endl;
    } else {
        Error("IsVard");
    }
}

void IsRefun() {
    if(tokens[index].first == INTTK || tokens[index].first == CHARTK) {
        IsStah();
        OutToken(); // '('
        index++;
        IsPara();
        OutToken(); // ')'
        index++;
        OutToken(); // '{'
        index++;
        IsComse();
        OutToken(); // '}'
        index++;
        cout << "有返回值函数定义" << endl;
    } else {
        Error("IsRefun");
    }
}

void IsVofun() {
    if(tokens[index].first == VOIDTK) {
        OutToken(); // void
        index++;
        OutToken(); //标识符
        index++;
        OutToken(); // '('
        index++;
        IsPara();
        OutToken(); // ')'
        index++;
        OutToken(); // '{'
        index++;
        IsComse();
        OutToken(); // '}'
        cout << "<无返回值函数定义>" << endl;
    } else {
        Error("IsVofun");
    }
}

void IsComse() {
    if(tokens[index].first == CONSTTK) {
        IsCs();
    }
    if((tokens[index].first == INTTK || tokens[index].first == CHARTK) && tokens[index + 2].first != LPARENT) {
        IsVars();
    }
    IsSeli();
    cout << "<复合语句>" << endl;
}

void IsPara() {
    if(tokens[index].first == INTTK || tokens[index].first == CHARTK) {
        OutToken(); // int | char
        index++;
        OutToken(); // 标识符
        index++;
        while(tokens[index].first == COMMA) {
            OutToken(); // ','
            index++;
            OutToken(); // int | char
            index++;
            OutToken(); // 标识符
            index++;
        }
        cout << "<参数表>" << endl;
    }
}

void IsMain() {
    if(tokens[index].first == VOIDTK) {
        OutToken(); // void
        index++;
        OutToken(); // main 
        index++;
        OutToken(); // '('
        index++;
        OutToken(); // ')'
        index++;
        OutToken(); // '{'
        index++;
        IsComse();
        OutToken(); // '}'
        index++;
        cout << "<主函数>" << endl;
    } else {
        Error("IsMain");
    }
}

void IsExp() {
    if(tokens[index].first == PLUS || tokens[index].first == MINU) {
        OutToken(); // '+' | '-' 
        index++;
    }
    IsXi();
    while(tokens[index].first == PLUS || tokens[index].first == MINU) {
        OutToken(); // '+' | '-'
        index++;
        IsXi();
    }
    cout << "<表达式>" << endl;
}

void IsXi() {
    IsTra();
    while(tokens[index].first == MULT || tokens[index].first == DIV) {
        OutToken(); // '*' | '/'
        index++;
        IsTra();
    }
    cout << "<项>" << endl;
}

void IsTra() {
    if(tokens[index].first == IDENFR && tokens[index + 1].first != LPARENT) {
        OutToken(); //标识符
        index++;
        if(tokens[index].first == LBRACK) {
            OutToken(); // '['
            index++;
            IsExp();
            OutToken(); // ']'
            index++;
        }
        cout << "<因子>" << endl;
    } else if(tokens[index].first == LPARENT) {
        OutToken(); // '('
        index++;
        IsExp();
        OutToken(); // ')'
        index++;
        cout << "<因子>" << endl;
    } else if(tokens[index].first == PLUS || tokens[index].first == MINU || tokens[index].first == INTCON) {
        IsInt();
        cout << "<因子>" << endl;
    } else if(tokens[index].first == CHARCON) {
        OutToken(); // 字符
        index++;
        cout << "<因子>" << endl;
    } else if(tokens[index].first == IDENFR && tokens[index + 1].first == LPARENT) {
        IsRefun();
        cout << "<因子>" << endl;
    } else {
        Error("IsTra");
    }
}

void IsSen() {
    if(tokens[index].first == IFTK) {
        IsIfsen();
        cout << "<语句>" << endl;
    } else if(tokens[index].first == WHILETK || tokens[index].first == DOTK || tokens[index].first == FORTK) {
        IsLoop();
        cout << "<语句>" << endl;
    } else if(tokens[index].first == LBRACE) {
        OutToken(); //'{'
        index++;
        IsSeli();
        OutToken(); //'}'
        index++;
        cout << "<语句>" << endl;
    } else if(tokens[index].first == IDENFR && tokens[index + 1].first != ASSIGN) {
        IsRefunu();
        OutToken(); // ';'
        index++;
        cout << "<语句>" << endl;
    } else if(tokens[index].first == IDENFR && tokens[index + 1].first == ASSIGN) {
        IsEqsen();
        OutToken(); // ';'
        index++;
        cout << "<语句>" << endl;
    } else if(tokens[index].first == SCANFTK) {
        IsReadse();
        OutToken(); // ';'
        index++;
        cout << "<语句>" << endl;
    } else if(tokens[index].first == SCANFTK) {
        IsWrise();
        OutToken(); // ';'
        index++;
        cout << "<语句>" << endl;
    } else if(tokens[index].first == SEMICN) {
        OutToken(); // ';'
        index++;
        cout << "<语句>" << endl;
    } else if(tokens[index].first == RETURNTK) {
        IsReadse();
        OutToken(); // ';'
        index++;
        cout << "<语句>" << endl;
    } else {
        Error("IsSen");
    }
}


