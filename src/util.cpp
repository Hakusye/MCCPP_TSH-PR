#include "util.hpp"

using namespace std;
//weightはあえて符号逆(頂点数は多い方が、重みは小さい方が良いものなので)
bool ColorClass::operator< (const ColorClass &another_color_class) const {
    if(vertexes.size() != another_color_class.vertexes.size()) {
        return vertexes.size() < another_color_class.vertexes.size();
    }else {
        return weight < another_color_class.weight;
    }
}
bool ColorClass::operator> (const ColorClass &another_color_class) const {
    if(vertexes.size() != another_color_class.vertexes.size()) {
        return vertexes.size() > another_color_class.vertexes.size();
    }else {
        return weight < another_color_class.weight;
    }
}
bool ColorSet::operator< (const ColorSet &another_color_set) const {
    return score < another_color_set.score;
}
bool ColorSet::operator> (const ColorSet &another_color_set) const {
    return score > another_color_set.score;
}
bool ColorSet::operator== (const ColorSet &another_color_set) const {
    return ( search_color.size() == another_color_set.search_color.size() ) &&
            equal( search_color.begin(),search_color.end() ,
            another_color_set.search_color.begin(),another_color_set.search_color.end() );
}
bool VertexMove::operator> (const VertexMove &another_vertex_move) const {
    if(vertex != another_vertex_move.vertex) return vertex > another_vertex_move.vertex;
    return color > another_vertex_move.color;
}
bool VertexMove::operator< (const VertexMove &another_vertex_move) const {
    if(vertex != another_vertex_move.vertex) return vertex < another_vertex_move.vertex;
    return color < another_vertex_move.color;
}

bool pair_set_int_comp (const pair<set<int>,int> &left,const pair<set<int>,int> &right){
    return left.first.size() < right.first.size();
}
bool pair_set_int_comp_greater (const pair<set<int>,int> &left,const pair<set<int>,int> &right){
    return left.first.size() > right.first.size();
}
bool set_comp (const set<int> &left,const set<int> &right) {
    return left.size() < right.size();
}
bool set_comp_greater (const set<int> &left,const set<int> &right) {
    return left.size() > right.size();
}
bool set_int_equal (const set<int> &left,const set<int> &right) {
    return left.size() == right.size() && equal(left.begin(),left.end(),right.begin(),right.end());
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
vector<int> shuffle(vector<int> target,mt19937 random_device) {
    
    for ( int i=0; i < target.size(); i++) {
        swap( target[i]  , target[ random_device() % target.size() ] );
    }
    return target;
}