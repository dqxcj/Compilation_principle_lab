#include <iostream>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "lab2.h"
using namespace std;

unordered_map<int, vector<vector<int>>> grammar = {  //此处修改
    {PROCGR, {{MAINGR}, {CSGR, MAINGR}, {CDGR, MAINGR}, {CSGR, CDGR, MAINGR}, {CSGR, TFUNGR, MAINGR}, {CDGR, TFUNGR, MAINGR}, {CSGR, CDGR, TFUNGR, MAINGR}, {TFUNGR, MAINGR}}}, //程序
    {TFUNGR, {{REFUNGR, TFUNGR}, {VOFUNGR, TFUNGR}, {NU}}}, //{有返回值函数定义|无返回值函数定义}
    {CSGR, {{CONSTTK, CDGR, SENGR, CSGRH}}}, //常量说明
    {CSGRH, {{CONSTTK, CDGR, SENGR, CSGRH}, {NU}}}, //辅助常量说明
    {CDGR, {{INTTK, IDENFR, ASSIGN, INTGR, CDGRH1}, {CHARTK, IDENFR, ASSIGN, CHARCON, CDGRH2}}}, //常量定义
    {CDGRH1, {{IDENFR, ASSIGN, INTGR, CDGRH1}, {NU}}}, //辅助常量定义1
    {CDGRH2, {{IDENFR, ASSIGN, CHARCON, CDGRH2}, {NU}}},
    {INTGR, {{UINTGR}, {PLUS, UINTGR}}, {MINU, UINTGR}},
    {STAHGR, {{INTTK, IDENFR}, {CHARTK, IDENFR}}},
    {VARSGR, {{VARDGR, SEMICN, VARSGRH}}},
    {VARSGRH, {{VARDGR, SEMICN, VARSGRH}, {NU}}},
    {VARDGR}
};

unordered_map<int, string> re = {  //此处修改
    {E, "E"},
    {T, "T"},
    {E1, "E1"},
    {NU, "NU"},
    {F, "F"},
    {T1, "T1"},
    {Jia, "Jia"},
    {Chen, "Chen"},
    {I, "I"},
    {Lkh, "Lkh"},
    {Rkh, "Rkh"}
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
    int i = 0;
    for(auto &val : first[vec[0]]) {
        vFirst.push_back(val);
    }
    while(i + 1 < vec.size() && HaveVNull(vec[i]) && IsNotEnd(vec[i])) {
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
    if(node == NULL) {
        return ;
    }
    for(auto child : node -> children_) {
        OutputTree(child);
    }
    cout << re[node -> val_] << endl;
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
    follow[E].insert(NU);  //此处修改
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
    // for(auto &p1 : predictAnalyzeTable) {
    //     cout << p1.first << "\t";
    //     for(auto &p2 : p1.second) {
    //         cout << "\tEnd" << p2.first << "\t";
    //         for(auto &i : p2.second) {
    //             cout << i << " ";
    //         }
    //     }
    //     cout << endl;
    // }

    vector<int> input = {I, Chen, I, Jia, I};  //此处修改
    vector<int> analyze = {E};  //此处修改
    deque<vector<vector<int>>> lexeme = GetLexeme(input, analyze);
    // for(auto &vec : lexeme) {
    //     cout << vec[0][0] << "\t->\t";
    //     for(auto &i : vec[1]) {
    //         cout << i << " ";
    //     }
    //     cout << endl;
    // }
    shared_ptr<Node> root = GetTree(lexeme, lexeme[0][0][0]);

    OutputTree(root);

    return 0;
}

