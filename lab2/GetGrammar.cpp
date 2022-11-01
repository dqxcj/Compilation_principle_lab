#include <unordered_map>
#include <vector>
#include "lab2.h"
using namespace std;


int main() {
    unordered_multimap<int, vector<int>> grammarSet = {
        {ADDGR, {MUSTGR}},
        {MULGR, {MUSTGR}},
        {RELAGR, {MUSTGR}},
        {ALPHGR, {MUSTGR}},
        {NUMGR, {MUSTGR}},
        {NUMGR, {MUSTGR, NZNUMGR}},
        {NZNUMGR, {MUSTGR}},
        {CHARGR, {MUSTGR, ADDGR}},
        {CHARGR, {MUSTGR, MULGR}},
        {CHARGR, {MUSTGR, ALPHGR}},
        {CHARGR, {MUSTGR, NUMGR}},
        {STRGR, {MUSTGR}},
        {PROCGR, {}}
    };
    return 0;
}