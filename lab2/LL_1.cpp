#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "lab1.h"
#include "lab2.h"
using namespace std;

unordered_map<int, vector<vector<int>>> grammar = {  //此处修改
    {PROCGR, {{MAINGR}, {CSGR, MAINGR}, {CDGR, MAINGR}, {CSGR, CDGR, MAINGR}, {CSGR, TFUNGR, MAINGR}, {CDGR, TFUNGR, MAINGR}, {CSGR, CDGR, TFUNGR, MAINGR}, {TFUNGR, MAINGR}}}, //程序
    {TFUNGR, {{REFUNGR, TFUNGR}, {VOFUNGR, TFUNGR}, {NU}}}, //{有返回值函数定义|无返回值函数定义}
    {CSGR, {{CONSTTK, CDGR, SEMICN, CSGRH}}}, //常量说明
    {CSGRH, {{CONSTTK, CDGR, SEMICN, CSGRH}, {NU}}}, //辅助常量说明
    {CDGR, {{INTTK, IDENFR, ASSIGN, INTGR, CDGRH1}, {CHARTK, IDENFR, ASSIGN, CHARCON, CDGRH2}}}, //常量定义
    {CDGRH1, {{COMMA, IDENFR, ASSIGN, INTGR, CDGRH1}, {NU}}}, //辅助常量定义1
    {CDGRH2, {{COMMA, IDENFR, ASSIGN, CHARCON, CDGRH2}, {NU}}},
    {UINTGR, {{INTCON}}},
    {INTGR, {{UINTGR}, {PLUS, UINTGR}, {MINU, UINTGR}}},
    {STAHGR, {{INTTK, IDENFR}, {CHARTK, IDENFR}}},
    {VARSGR, {{VARDGR, SEMICN, VARSGRH}}},
    {VARSGRH, {{VARDGR, SEMICN, VARSGRH}, {NU}}},
    {VARDGR, {{TYIDGR, IDENFR, VARDGRH}, {TYIDGR, IDENFR, LBRACK, UINTGR, RBRACK, VARDGRH}}},
    {VARDGRH, {{COMMA, IDENFR, VARDGRH}, {COMMA, IDENFR, LBRACK, UINTGR, RBRACK, VARDGRH}, {NU}}},
    {REFUNGR, {{STAHGR, LPARENT, PARAGR, RPARENT, LBRACE, COMSEGR, RBRACE}}},
    {VOFUNGR, {{VOIDTK, IDENFR, LPARENT, PARAGR, RPARENT, LBRACE, COMSEGR, RBRACE}}},
    {COMSEGR, {{SELIGR}, {CSGR, SELIGR}, {VARSGR, SELIGR}, {CSGR, VARSGR, SELIGR}}},
    {PARAGR, {{TYIDGR, IDENFR, PARAGR}, {TYIDGR, IDENFR, COMMA, PARAGR},{NU}}},
    {MAINGR, {{VOIDTK, MAINTK, LPARENT, RPARENT, LBRACE, COMSEGR, RBRACE}}},
    {EXPGR, {{XIGR, EXPGRH}, {PLUS, XIGR, EXPGRH}, {MINU, XIGR, EXPGRH}}},
    {EXPGRH, {{PLUS, XIGR, EXPGRH}, {MINU, XIGR, EXPGRH}, {NU}}}, 
    {XIGR, {{TRAGR, XIGRH}}},
    {XIGRH, {{NU}, {MULT, TRAGR, XIGRH}, {DIV, TRAGR, XIGRH}}},
    {TRAGR, {{IDENFR}, {IDENFR, LBRACK, EXPGR, RBRACK}, {LPARENT, EXPGR, RPARENT}, {INTGR}, {CHARCON}, {REFUNUGR}}},
    {SENGR, {{IFSENGR}, {LOOPGR}, {LBRACE, SELIGR, RBRACE}, {REFUNUGR, SEMICN}, {VOFUNUGR, SEMICN}, {EQSENGR, SEMICN}, {READSEGR, SEMICN}, {WRISEGR, SEMICN}, {NU, SEMICN}, {RETUSEGR, SEMICN}}},
    {EQSENGR, {{IDENFR, ASSIGN, EXPGR}, {IDENFR, LBRACK, EXPGR, RBRACK, ASSIGN, EXPGR}}},
    {IFSENGR, {{IFTK, LPARENT, IFGR, RPARENT, SENGR}, {IFTK, LPARENT, IFGR, RPARENT, SENGR, ELSETK, SENGR}}},
    {IFGR, {{EXPGR, IFGRH, EXPGR}, {EXPGR}}},
    {IFGRH, {{LSS}, {LEQ}, {GRE}, {GEQ}, {NEQ}, {EQL}}},
    {LOOPGR, {{WHILETK, LPARENT, IFGR, RPARENT, SENGR}, {DOTK, SENGR, WHILETK, LPARENT, IFGR, RPARENT}, {FORTK, LPARENT, IDENFR, ASSIGN, EXPGR, SEMICN, IFGR, SEMICN, IDENFR, ASSIGN, IDENFR, PLUS, WLENGR, RPARENT, SENGR}, {FORTK, LPARENT, IDENFR, ASSIGN, EXPGR, SEMICN, IFGR, SEMICN, IDENFR, ASSIGN, IDENFR, MINU, WLENGR, RPARENT, SENGR}}}, 
    {WLENGR, {{UINTGR}}},
    {REFUNUGR, {{IDENFR, LPARENT, VPALIGR, RPARENT}}},
    {VOFUNUGR, {{IDENFR, LPARENT, VPALIGR, RPARENT}}},
    {VPALIGR, {{EXPGR}, {EXPGR, COMMA}, {NU}}},
    {SELIGR, {{SENGR, SELIGR}, {NU}}},
    {READSEGR, {{SCANFTK, LPARENT, READSEGRH, RPARENT}}},
    {READSEGRH, {{NU}, {COMMA, IDENFR, READSEGRH}}},
    {WRISEGR, {{PRINTFTK, LPARENT, STRCON, EXPGR, RPARENT}, {PRINTFTK, LPARENT, STRCON, RPARENT}, {PRINTFTK, LPARENT, EXPGR, RPARENT}}},
    {RETUSEGR, {{RETURNTK}, {RETURNTK, LPARENT, EXPGR, RPARENT}}}
};

unordered_map<string, int> inputRefer = {
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

unordered_map<int, string> re = {
    {STRCON, "字符串"},
    {PROCGR, "程序"},
    {CSGR, "常量说明"},
    {CDGR, "常量定义"},
    {UINTGR, "无符号整数"},
    {INTGR, "整数"},
    {STAHGR, "声明头部"},
    {VARSGR, "变量说明"},
    {VARDGR, "变量定义"},
    {REFUNGR, "有返回值函数定义"},
    {VOFUNGR, "无返回值函数定义"},
    {COMSEGR, "复合语句"},
    {PARAGR, "参数表"},
    {MAINGR, "主函数"},
    {EXPGR, "表达式"},
    {XIGR, "项"},
    {TRAGR, "因子"},
    {SENGR, "语句"},
    {EQSENGR, "赋值语句"},
    {IFSENGR, "条件语句"},
    {IFGR, "条件"},
    {LOOPGR, "循环语句"},
    {WLENGR, "步长"},
    {REFUNUGR, "有返回值函数调用语句"},
    {VOFUNUGR, "无返回值函数调用语句"},
    {VPALIGR, "值参数表"},
    {SELIGR, "语句列"},
    {READSEGR, "读语句"},
    {WRISEGR, "写语句"},
    {RETUSEGR, "返回语句"},

    {IDENFR, "IDENFR"},
    {INTCON, "INTCON"},
    {CHARCON, "CHARCON"},
    {STRCON, "STRCON"},
    {CONSTTK, "CONSTTK"},
    {INTTK, "INTTK"},
    {CHARTK, "CHARTK"},
    {VOIDTK, "VOIDTK"},
    {MAINTK, "MAINTK"},
    {IFTK, "IFTK"},
    {ELSETK, "ELSETK"},
    {DOTK, "DOTK"},
    {WHILETK, "WHILETK"},
    {FORTK, "FORTK"},
    {SCANFTK, "SCANFTK"},
    {PRINTFTK, "PRINTFTK"},
    {RETURNTK, "RETURNTK"},
    {PLUS, "PLUS"},
    {MINU, "MINU"},
    {MULT, "MULT"},
    {DIV, "DIV"},
    {LSS, "LSS"},
    {LEQ, "LEQ"},
    {GRE, "GRE"},
    {GEQ, "GEQ"},
    {EQL, "EQL"},
    {NEQ, "NEQ"},
    {ASSIGN, "ASSIGN"},
    {SEMICN, "SEMICN"},
    {COMMA, "COMMA"},
    {LPARENT, "LPARENT"},
    {RPARENT, "RPARENT"},
    {LBRACK, "LBRACK"},
    {RBRACK, "RBRACK"},
    {LBRACE, "LBRACE"},
    {RBRACE, "RBRACE"},
};

class Node
{
public:
    Node();
    Node(int val): val_(val) {}

    int val_ = 0;
    vector<shared_ptr<Node>> children_;
};



unordered_map<int, unordered_set<int>> first;
unordered_map<int, unordered_set<int>> follow;
unordered_map<int, unordered_map<int, vector<int>>> predictAnalyzeTable;

bool IsNotEnd(int val) {
    if(grammar.find(val) != grammar.cend()) {
        return true;
    }
    return false;
}

bool NextIsNull(vector<int> &curV, vector<int>::const_iterator cur) {
    vector<int> nullV = {NU};
    bool flag = false;
    while(cur != curV.cend()) {
        if(!IsNotEnd(*cur) && *cur != NU) {
            return false;
        }
        for(auto &v : grammar[*cur]) {
            if(v == nullV) {
                flag = true;
            }
        }
        if(!flag) {
            return false;
        }
        flag = false;
        ++cur;
    }
    return true;
}

void GetFirst(const int notEnd) {
    for(auto &v : grammar[notEnd]) {
        if(IsNotEnd(v[0])) {
            GetFirst(v[0]);
            for(auto &i : first[v[0]]) {
                first[notEnd].insert(i);
            }
        } else {
            first[notEnd].insert(v[0]);
        }
    }
}

void GetFollow() {
    for(auto &p : grammar) {
        for(auto &v : p.second) {
            for(auto it = v.cbegin(); it != v.cend(); ++it) {
                if(IsNotEnd(*it)) {
                    if(it + 1 != v.cend()) {
                        if(IsNotEnd(*(it + 1))) {
                            for(auto &i : first[*(it + 1)]) {
                                follow[*it].insert(i);
                            }
                        } else {
                            follow[*it].insert(*(it + 1));
                        }
                    } else {
                        if(follow.find(p.first) != follow.cend()) {
                            for(auto &i : follow[p.first]) {
                                follow[*it].insert(i);
                            }
                        }
                    }
                    if(NextIsNull(v, it + 1)) {
                        if(follow.find(p.first) != follow.cend()) {
                            for(auto &i : follow[p.first]) {
                                follow[*it].insert(i);
                            }
                        }
                    }
                }
            }
        }
    }
}

bool HaveVNull(int val) {
    vector<int> vNull = {NU};
    for(auto &v : grammar[val]) {
        if(v == vNull) {
            return true;
        }
    }
    return false;
}

vector<int> GetVFirst(vector<int> &vec) {
    vector<int> vFirst;
    long long i = 0;
    for(auto &val : first[vec[0]]) {
        vFirst.push_back(val);
    }
    while(i + 1 < int(vec.size()) && HaveVNull(vec[i]) && IsNotEnd(vec[i])) {
        i++;
        for(auto &val : first[vec[i]]) {
            vFirst.push_back(val);
        }
    }
    if(!IsNotEnd(vec[i])) {
        vFirst.push_back(vec[i]);
    }
    return vFirst;
}

vector<int> GetSelect(const int notEnd, vector<int> &right) {
    vector<int> select;
    if(IsNotEnd(right[0])) {
        if(NextIsNull(right, right.begin())) {
            for(auto &i : GetVFirst(right)) {
                if(i != NU) {
                    select.push_back(i);
                }
            }
            for(auto &i : follow[notEnd]) {
                select.push_back(i);
            }
        } else {
            for(auto &i : GetVFirst(right)) {
                select.push_back(i);
            }
        }
    } else {
        if(right[0] == NU) {
            for(auto &i : follow[notEnd]) {
                select.push_back(i);
            }
        } else {
            select.push_back(right[0]);
        }
    }

    return select;
}

deque<vector<vector<int>>> GetLexeme(vector<int> &input, vector<int> &analyze) {
    deque<vector<vector<int>>> lexeme;
    while(input.size() != 0 || analyze.size() != 0) {
        // cout << "c\n";
        vector<int> childrenValV;
        if(input.size() > 0 && predictAnalyzeTable.find(analyze.back()) != predictAnalyzeTable.end()) {
            childrenValV = predictAnalyzeTable[analyze.back()][input.back()];
            lexeme.push_back({{analyze.back()}, {childrenValV}});
        }
        if(input.size() == 0) {
            lexeme.push_back({{analyze.back()}, {{NU}}});
        }
        if(input.size() > 0 && input.back() == analyze.back()) {
            input.pop_back();
        }
        analyze.pop_back();
        for(auto it = childrenValV.crbegin(); it != childrenValV.crend(); ++it) {
            analyze.push_back(*it);
        }
    }
    // cout << "d\n";
    return lexeme;
}

shared_ptr<Node> GetTree(deque<vector<vector<int>>> &lexeme, int val) {
    int nodeVal = val;
    vector<int> nodeChildren;
    shared_ptr<Node> node = NULL;

    if(IsNotEnd(nodeVal)) {
        nodeVal = lexeme[0][0][0];
        nodeChildren = lexeme[0][1];
        lexeme.pop_front();
    }
    if(nodeVal != NU) 
        node = make_shared<Node>(nodeVal);
    for(auto &i : nodeChildren) {
        node -> children_.push_back(GetTree(lexeme, i));
    }
    return node;
}

void OutputTree(shared_ptr<Node> node) {
    ofstream out("output.txt");
    if(node == NULL) {
        return ;
    }
    for(auto child : node -> children_) {
        OutputTree(child);
    }
    out << re[node -> val_] << endl;
}

int main() {
    for(auto &p : grammar) {
        if(first.find(p.first) == first.end()) {
            GetFirst(p.first);
        }
    }
    // for(auto &p : first) {
    //     cout << p.first << '\t';
    //     for(auto &i : p.second) {
    //         cout << i << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    follow[PROCGR].insert(NU);  //此处修改
    GetFollow();
    GetFollow();
    // for(auto &p : follow) {
    //     cout << p.first << "\t";
    //     for(auto &i : p.second) {
    //         cout << i << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    for(auto &p : grammar) {
        for(auto &v : p.second) {
            vector<int> select = GetSelect(p.first, v);
            for(auto &i : select) {
                predictAnalyzeTable[p.first].emplace(i, v);
            }
        }
    }
    // int i = 0;
    // for(auto &p1 : predictAnalyzeTable) {
    //     if(i++ > 50) break;
    //     if(re.find(p1.first) != re.cend()) cout << re[p1.first] << "\t";
    //     for(auto &p2 : p1.second) {
    //         if(re.find(p2.first) != re.cend()) cout << "\tEnd" << re[p2.first] << "\t";
    //         for(auto &i : p2.second) {
    //             if(re.find(i) != re.cend()) cout << re[i] << " ";
    //         }
    //     }
    //     cout << endl;
    // }

    lab1();
    ifstream in("laboutput.txt");
    vector<int> input; 
    string word;
    while(in >> word) {
        if(inputRefer.find(word) != inputRefer.end()) {
            input.push_back(inputRefer[word]);
        } else {
            // cout << "inputRefer没有" << word << endl;
        }
    }

    // cout << "a\n";
    vector<int> analyze = {PROCGR}; 
    deque<vector<vector<int>>> lexeme = GetLexeme(input, analyze);
    // for(auto &vec : lexeme) {
    //     cout << vec[0][0] << "\t->\t";
    //     for(auto &i : vec[1]) {
    //         cout << i << " ";
    //     }
    //     cout << endl;
    // }
    // cout << "b\n";
    shared_ptr<Node> root = GetTree(lexeme, lexeme[0][0][0]);

    // cout <<"line427";
    OutputTree(root);

    return 0;
}

