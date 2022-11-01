#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "lab2.h"
using namespace std;



unordered_map<int, vector<vector<int>>> grammar = {
    {E, {{T, E1}}},
    {E1, {{Jia, T, E1}, {NU}}},
    {T, {{F, T1}}},
    {T1, {{Chen, F, T1}, {NU}}},
    {F, {{I}, {Lkh, E, Rkh}}}
};

unordered_map<int, unordered_set<int>> first;
unordered_map<int, unordered_set<int>> follow;
unordered_map<int, unordered_multimap<int, vector<int>>> predictAnalyzeTable;

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

void GetLexeme(vector<int> &input, vector<int> &analyze, unordered_map<int, unordered_multimap<int, vector<int>>> &PAT) {
    return ;
}

int main() {
    for(auto &p : grammar) {
        if(first.find(p.first) == first.end()) {
            GetFirst(p.first);
        }
    }
    for(auto &p : first) {
        cout << p.first << '\t';
        for(auto &i : p.second) {
            cout << i << " ";
        }
        cout << endl;
    }
    cout << endl;
    follow[E].insert(NU);
    GetFollow();
    GetFollow();
    for(auto &p : follow) {
        cout << p.first << "\t";
        for(auto &i : p.second) {
            cout << i << " ";
        }
        cout << endl;
    }
    for(auto &p : grammar) {
        for(auto &v : p.second) {
            vector<int> select = GetSelect(p.first, v);
            for(auto &i : select) {
                predictAnalyzeTable[p.first].emplace(i, v);
            }
        }
    }
    for(auto &p1 : predictAnalyzeTable) {
        cout << p1.first << "\t";
        for(auto &p2 : p1.second) {
            cout << "\tEnd" << p2.first << "\t";
            for(auto &i : p2.second) {
                cout << i << " ";
            }
        }
        cout << endl;
    }

    vector<int> input = {I, Jia, I, Chen, I};
    vector<int> analyze = {E};
    GetLexeme(input, analyze, predictAnalyzeTable);

    return 0;
}

