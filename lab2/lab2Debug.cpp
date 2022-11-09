#include <iostream>
#include <vector>
#include <utility>
#include <string>
using namespace std;

void OutputTokens(const vector<pair<int, string>> &tokens) {
    for(auto &p : tokens) {
        cout << p.first << " " <<  p.second << endl;
    }
}