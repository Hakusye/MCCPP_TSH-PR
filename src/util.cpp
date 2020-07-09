#include <set>
#include <string>
#include <vector>
#include <map>
#include "util.hpp"

using namespace std;

bool operator< (const ColorSet &c1, const ColorSet &c2) { return c1.score > c2.score; }

vector<string> split(string str,char del) {
    int first = 0;
    int last = str.find_first_of(del);
    vector<string> ans;
    while(first < str.size()) {
        string word(str, first, last - first);
        ans.push_back(word);
        first = last + 1;
        last = str.find_first_of(del,first);
        if(last == string::npos) last = str.size();
    }
    return ans;
}