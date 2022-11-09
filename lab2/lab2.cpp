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
// 报错
void Error(const string &s);  
// <程序>
void IsProc();
// <常量说明>
void IsCs();
// <常量定义>
void IsCd();
void IsUint();
void IsInt();
void IsIden();
void IsStah();
// <变量说明>
void IsVars();
void IsVard();
void IsTyid();
// <有返回值函数定义>
void IsRefun();
void IsVofun();
void IsComse();
void IsPara();
// <主函数>
void IsMain();
void IsExp();
void IsXi();
void IsTra();
void IsSen();
void IsEqsen();
void IsIfsen();
void IsIf();
void IsLoop();
void IsWlen();
void IsRefunu();
void IsVofunu();
void IsVpali();
void IsSeli();
void IsReadse();
void IsWrise();
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
    index++;
    IsCd();
    index++;  //跳过';'
    if(tokens[index].first == CONSTTK) {
        IsCs();  //递归模拟{const <常量定义> ;}
    }
}

void IsCd() {

}