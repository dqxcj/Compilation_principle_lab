#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

int main() {
    ifstream in("test.txt");
    ofstream out("testout.txt");
    string line;
    while(getline(in, line)) {
        string word;
        vector<string> temp;
        istringstream getword(line);
        while(getword >> word) {
            temp.push_back(word);
        }
        int i = 0;
        for(auto &c : temp[0]) {
            if(i++ > 2) {
                c = c + ('a' - 'A');
            }
        }
        out  << "void" << " " << temp[0] << "();\n";
    }
    return 0;
}