#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
//#include "lab1.h"
using namespace std;
unordered_map<string, string> classMap = {
    {"const", "CONSTTK"}, {"int", "INTTK"}, {"char", "CHARTK"},
    {"void", "VOIDTK"}, {"main", "MAINTK"}, {"if", "IFTK"}, 
    {"else", "ELSETK"}, {"do", "DOTK"}, {"while", "WHILETK"},
    {"for", "FORTK"}, {"scanf", "SCANFTK"}, {"printf", "PRINTFTK"},
    {"return", "RETURNTK"}, {"+", "PLUS"}, {"-", "MINU"}, {"*", "MULT"},
    {"/", "DIV"}, {"<", "LSS"}, {"<=", "LEQ"}, {">", "GRE"}, {">=", "GEQ"},
    {"==", "EQL"}, {"!=", "NEQ"}, {"=", "ASSIGN"}, {";", "SEMICN"}, 
    {",", "COMMA"}, {"(", "LPARENT"}, {")", "RPARENT"}, {"[", "LBRACK"},
    {"]", "RBRACK"}, {"{", "LBRACE"}, {"}", "RBRACE"}
};


void processStr(string &s, vector<string> &vs) {
    bool flag = false;
    auto left = s.begin();
    for(auto it = s.begin(); it != s.end(); ++it) {
        if(*it == '"') {
            if(!flag) {
                left = it;
                flag = true;
            } else if(flag) {
                vs.push_back(string(left + 1, it));

                // replace str by "
                while(left != it + 1) {
                    *left++ = ' ';
                }
                it = s.insert(it, '"');
                ++it;
                flag = false;
            }
        }
    }
}

void processSpace(string &s) {
    for (auto it = s.begin(); it != s.end(); ++it){
        if(*it == '\'') {
            ++it;
            while(*it != '\'') {
                ++it;
            }
            ++it;
        }
        if(*it == '<' || *it == '>' || *it == '=' || *it == '!') {
            if(*(it + 1) != '=') {
                ;
            } else {
                it = s.insert(it, ' ');
                it += 3;
                it = s.insert(it, ' ');
                continue;
            }
        }
        if (classMap.find(string(1, *it)) != classMap.cend()){
            it = s.insert(it, ' ');
            it += 2;
            it = s.insert(it, ' ');
        }
    }
}


int main() {
    ifstream in("testfile.txt");
    ofstream out("output.txt");
    string line;
    while(getline(in, line)) {
        vector<string> strV;
        string temp;
        istringstream gettemp(line);
        gettemp >> temp;
        if(temp.size() >= 5) temp = string(temp.cbegin(), temp.cbegin() + 5);
        if(temp == "print") {
            // cout << line << endl;
            processStr(line, strV);
            // cout << line << endl;
        }
        processSpace(line);
        int strIndex = 0;
        string word;
        istringstream getword(line);
        while(getword >> word) {
            if(word == "\"") {
                out << "STRCON " << strV[strIndex] << endl;
                strIndex++;
                continue;
            }
            if(word.front() == '\'') {
                out << "CHARCON " << string(word.cbegin() + 1 , word.cend() - 1) << endl;
                continue;
            }
            if(classMap.find(word) != classMap.cend()) {
                out << classMap[word] << " " << word << endl;
            } else {
                bool isNum = true;
                for(auto &c: word) {
                    if(c > '9') {
                        isNum = false;
                        break;
                    }
                }
                if(isNum) {
                    out << "INTCON " << word << endl;
                } else {
                    out << "IDENFR " << word << endl;
                }
            }   
        }
    }
    return 0;
}