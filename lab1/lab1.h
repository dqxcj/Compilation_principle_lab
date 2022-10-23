#ifndef LAB1_H
#define LAB1_H
#include <unordered_map>
#include <string>

/*
const int IDENFR = 1;
const int InTCON = 2;
const int CHARCON = 3;
const int STRCON = 4;
const int CONSTTK = 5;
const int INTTK = 6;
const int CHARTK = 7;
const int VOIDTK = 8;
const int MAINTK = 9;
const int IFTK = 10;
const int ELSETK = 11;
const int DOTK = 12;
const int WHILETK = 13;
const int FORTK = 14;
const int SCANFTK = 15;
const int PRINTFTK = 16;
const int RETURNTK = 17;
const int PLUS = 18;
const int MINU = 19;
const int MULT = 20;
const int DIV = 21;
const int LSS = 22;
const int LEQ = 23;
const int GRE = 24;
const int GEQ = 25;
const int EQL = 26;
const int NEQ = 27;
const int ASSIGN = 28;
const int SEMICN = 29;
const int COMMA = 30;
const int LPARENT = 31;
const int RPARENT = 32;
const int LBRACK = 33;
const int RBRACK = 34;
const int LBRACE = 35;
const int RBRACE = 36;
*/

std::unordered_map<std::string, std::string> classMap = {
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

#endif