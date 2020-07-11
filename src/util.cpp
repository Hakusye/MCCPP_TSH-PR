#include "util.hpp"

using namespace std;

bool ColorClass::operator< (const ColorClass &another_color_class) const {
    return vertexes.size() < another_color_class.vertexes.size();
}
bool ColorClass::operator> (const ColorClass &another_color_class) const {
    return vertexes.size() > another_color_class.vertexes.size();
}

bool ColorSet::operator< (const ColorSet &another_color_set) const {
    return score < another_color_set.score;
}
bool ColorSet::operator> (const ColorSet &another_color_set) const {
    return score > another_color_set.score;
}

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