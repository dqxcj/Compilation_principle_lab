#ifndef LAB2STRUCT_H
#define LAB2STRUCT_H
#include "lab2.h"
#include <unordered_map>
#include <string>

std::unordered_map<std::string, int> inputRefer = {
    {"IDENFR", IDENFR},
    {"INTCON", INTCON},
    {"CHARCON", CHARCON},
    {"STRCON", STRCON},
    {"CONSTTK", CONSTTK},
    {"INTTK", INTTK},
    {"CHARTK", CHARTK},
    {"VOIDTK", VOIDTK},
    {"MAINTK", MAINTK},
    {"IFTK", IFTK},
    {"ELSETK", ELSETK},
    {"DOTK", DOTK},
    {"WHILETK", WHILETK},
    {"FORTK", FORTK},
    {"SCANFTK", SCANFTK},
    {"PRINTFTK", PRINTFTK},
    {"RETURNTK", RETURNTK},
    {"PLUS", PLUS},
    {"MINU", MINU},
    {"MULT", MULT},
    {"DIV", DIV},
    {"LSS", LSS},
    {"LEQ", LEQ},
    {"GRE", GRE},
    {"GEQ", GEQ},
    {"EQL", EQL},
    {"NEQ", NEQ},
    {"ASSIGN", ASSIGN},
    {"SEMICN", SEMICN},
    {"COMMA", COMMA},
    {"LPARENT", LPARENT},
    {"RPARENT", RPARENT},
    {"LBRACK", LBRACK},
    {"RBRACK", RBRACK},
    {"LBRACE", LBRACE},
    {"RBRACE", RBRACE}
};

#endif