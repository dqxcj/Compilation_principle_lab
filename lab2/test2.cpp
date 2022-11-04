#include <fstream>
using namespace std;

int main() {
    ifstream in("test.txt");
    ofstream out("testout.txt");
    string word;
    while(in >> word) {
        out <<"{"<< word << ", " << '"' << word << '"' << "},"<< endl; 
    }
    return 0;
}