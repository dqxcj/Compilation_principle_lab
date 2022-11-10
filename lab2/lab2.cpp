#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include "lab1.cpp"
#include "lab2Debug.cpp"
#include "lab2Struct.h"
using namespace std;

// pair.first��token������, pair.second��token����
vector<pair<int, string>> tokens;
// index��Ϊtokens��������ָ��ǰ�����token
int index = 0;
ifstream in("lab1output.txt");
ofstream out("output.txt");

// ��ȡtokens
void GetTokens(ifstream &in, vector<pair<int, string>> &tokens);
// �������token
void OutToken();
// ����
void Error(const string &s);  
// <����>
void IsProc();
// <����˵��>
void IsCs();
// <��������>
void IsCd();
// <�޷�������>
void IsUint();
// <����>
void IsInt();
// <����ͷ��>
void IsStah();
// <����˵��>
void IsVars();
// <��������>
void IsVard();
// <�з���ֵ��������>
void IsRefun();
// <�޷���ֵ��������>
void IsVofun();
// <�������>
void IsComse();
// <������>
void IsPara();
// <������>
void IsMain();
// <���ʽ>
void IsExp();
// <��>
void IsXi();
// <����>
void IsTra();
// <���>
void IsSen();
// <��ֵ���>
void IsEqsen();
// <�������>
void IsIfsen();
// <����>
void IsIf();
// <ѭ�����>
void IsLoop();
// <����>
void IsWlen();
// <�з���ֵ�����������>
void IsRefunu();
// <�޷���ֵ�����������>
void IsVofunu();
// <ֵ������>
void IsVpali();
// <�����>
void IsSeli();
// <�����>
void IsReadse();
// <д���>
void IsWrise();
// <�������>
void IsRetuse();

int main() {
    // ��Դ��ת��Ϊtokens������lab1output.txt
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
    // [����˵��]
    if(tokens[index].first == CONSTTK) {    
        IsCs();
    }
    // [����˵��] && ��һ��<�з���ֵ��������>
    if(tokens[index].first == INTTK || tokens[index].first == CHARTK) {   
        // ��һ��<�з���ֵ��������>�� �����λ����'('
        if(tokens[index + 2].first == LPARENT) {
            ;
        } else {    //[����˵��]
            IsVars();
        }
    }
    // {���з���ֵ�������壾|���޷���ֵ�������壾} && <������>
    while(tokens[index].first == INTTK || tokens[index].first == VOIDTK) {   
        // <������> && <�޷���ֵ��������>
        if(tokens[index].first == VOIDTK) {
            if (tokens[index + 1].first == MAINTK) {  //������
                IsMain();
                out << "<����>" << endl;
                return;
            } else {  // <�޷���ֵ��������>
                IsVofun();
            }
        } else if(tokens[index].first == INTTK) {  // <�з���ֵ��������>
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
    cout << "<����˵��>" << endl;
}

void IsCd() {
    if(tokens[index].first == INTTK) {
        OutToken(); //int
        index++;
        OutToken(); //��ʶ��
        index++;
        OutToken(); //'='
        index++;
        IsInt();
        while(tokens[index].first == COMMA) {
            OutToken(); // ','
            index++;
            OutToken(); // ��ʶ��
            index++;
            OutToken(); // '='
            index++;
            IsInt();
        }
        cout << "<��������>" << endl;
    } else if(tokens[index].first == CHARTK) {
        OutToken(); //char
        index++;
        OutToken(); //��ʶ��
        index++;
        OutToken(); //'='
        index++;
        OutToken(); //�ַ�
        index++;
        while(tokens[index].first == COMMA) {
            OutToken(); // ','
            index++;
            OutToken(); // ��ʶ��
            index++;
            OutToken(); // '='
            index++;
            OutToken(); // �ַ�
            index++;
        }
        cout << "<��������>" << endl;
    } else {
        Error("Iscd");
    }
}

void IsUint() {
    OutToken();
    index++;
    cout << "<�޷�������>" << endl;
}

void IsInt() {
    if(tokens[index].first == PLUS) {
        OutToken(); // '+'
        index++;
        IsUint();
        cout << "<����>" << endl;
    } else if(tokens[index].first == MINU) {
        OutToken(); // '-'
        index++;
        IsUint();
        cout << "<����>" << endl;
    } else if(tokens[index].first == INTCON) {
        IsUint();
        cout << "����" << endl;
    } else {
        Error("IsInt");
    }
}

void IsStah() {
    if(tokens[index].first == INTTK) {
        OutToken(); //int
        index++;
        OutToken(); //��ʶ��
        index++;
        cout << "<����ͷ��>" << endl;
    } else if(tokens[index].first == CHARTK) {
        OutToken(); // char
        index++;
        OutToken(); // ��ʶ��
        index++;
        cout << "<����ͷ��>" << endl;
    } else {
        Error("IsStah");
    }
}

void IsVars() {
    if(tokens[index].first == INTTK || tokens[index].first == CHARTK) {
        IsVard();
        OutToken(); // ';'
        index++;
        while((tokens[index].first == INTTK || tokens[index].first == CHARTK) && tokens[index + 2].first != LPARENT) { // != '(' �ų�����IsVars()���з���ֵ��������ĸ���
            IsVard();
            OutToken(); // ';'
            index++;
        }
        cout << "<����˵��>" << endl;
    } else {
        Error("IsVars");
    }
}

void IsVard() {
    if(tokens[index].first == INTTK || tokens[index].first == CHARTK) {
        OutToken(); // int | char
        index++;
        OutToken(); // ��ʶ��
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
            OutToken(); // ��ʶ��
            index++;
            if(tokens[index].first == LBRACK) {
                OutToken(); // '['
                index++;
                IsUint();
                OutToken(); // ']'
                index++;
            }
        }
        cout << "<��������>" << endl;
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
        cout << "�з���ֵ��������" << endl;
    } else {
        Error("IsRefun");
    }
}

void IsVofun() {
    if(tokens[index].first == VOIDTK) {
        OutToken(); // void
        index++;
        OutToken(); //��ʶ��
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
        cout << "<�޷���ֵ��������>" << endl;
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
    cout << "<�������>" << endl;
}

void IsPara() {
    if(tokens[index].first == INTTK || tokens[index].first == CHARTK) {
        OutToken(); // int | char
        index++;
        OutToken(); // ��ʶ��
        index++;
        while(tokens[index].first == COMMA) {
            OutToken(); // ','
            index++;
            OutToken(); // int | char
            index++;
            OutToken(); // ��ʶ��
            index++;
        }
        cout << "<������>" << endl;
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
        cout << "<������>" << endl;
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
    cout << "<���ʽ>" << endl;
}

void IsXi() {
    IsTra();
    while(tokens[index].first == MULT || tokens[index].first == DIV) {
        OutToken(); // '*' | '/'
        index++;
        IsTra();
    }
    cout << "<��>" << endl;
}

void IsTra() {
    if(tokens[index].first == IDENFR && tokens[index + 1].first != LPARENT) {
        OutToken(); //��ʶ��
        index++;
        if(tokens[index].first == LBRACK) {
            OutToken(); // '['
            index++;
            IsExp();
            OutToken(); // ']'
            index++;
        }
        cout << "<����>" << endl;
    } else if(tokens[index].first == LPARENT) {
        OutToken(); // '('
        index++;
        IsExp();
        OutToken(); // ')'
        index++;
        cout << "<����>" << endl;
    } else if(tokens[index].first == PLUS || tokens[index].first == MINU || tokens[index].first == INTCON) {
        IsInt();
        cout << "<����>" << endl;
    } else if(tokens[index].first == CHARCON) {
        OutToken(); // �ַ�
        index++;
        cout << "<����>" << endl;
    } else if(tokens[index].first == IDENFR && tokens[index + 1].first == LPARENT) {
        IsRefun();
        cout << "<����>" << endl;
    } else {
        Error("IsTra");
    }
}

void IsSen() {
    if(tokens[index].first == IFTK) {
        IsIfsen();
        cout << "<���>" << endl;
    } else if(tokens[index].first == WHILETK || tokens[index].first == DOTK || tokens[index].first == FORTK) {
        IsLoop();
        cout << "<���>" << endl;
    } else if(tokens[index].first == LBRACE) {
        OutToken(); //'{'
        index++;
        IsSeli();
        OutToken(); //'}'
        index++;
        cout << "<���>" << endl;
    } else if(tokens[index].first == IDENFR && tokens[index + 1].first != ASSIGN) {
        IsRefunu();
        OutToken(); // ';'
        index++;
        cout << "<���>" << endl;
    } else if(tokens[index].first == IDENFR && tokens[index + 1].first == ASSIGN) {
        IsEqsen();
        OutToken(); // ';'
        index++;
        cout << "<���>" << endl;
    } else if(tokens[index].first == SCANFTK) {
        IsReadse();
        OutToken(); // ';'
        index++;
        cout << "<���>" << endl;
    } else if(tokens[index].first == SCANFTK) {
        IsWrise();
        OutToken(); // ';'
        index++;
        cout << "<���>" << endl;
    } else if(tokens[index].first == SEMICN) {
        OutToken(); // ';'
        index++;
        cout << "<���>" << endl;
    } else if(tokens[index].first == RETURNTK) {
        IsReadse();
        OutToken(); // ';'
        index++;
        cout << "<���>" << endl;
    } else {
        Error("IsSen");
    }
}


