#include "../include/util.hpp"

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
void ColorSet::operator= (const ColorSet &copy_color_set) {
    this->S = copy_color_set.S;
    this->search_color = copy_color_set.search_color;
    this->score = copy_color_set.score;
    this->num_color = copy_color_set.num_color;
};
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
    //return left.size() == right.size() && equal(left.begin(),left.end(),right.begin());
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

inline vector<ColorClass> ColorSet::GetColorSet() const {
    return S;
}

inline map< int, int > ColorSet::GetSearchColor() const {
    return search_color;
}

void ColorSet::GreaterSort() {
    sort(S.begin(),S.end(),greater<ColorClass>());
    for( int i=0; i < S.size(); i++ ) {
        if(S[i].vertexes.empty()) break;
        for( int vertex : S[i].vertexes ) {
            this->search_color[vertex] = i;
        }
    }
}

void ColorSet::InitColorSet(ColorSet copy_color_set) {
    this->S = copy_color_set.GetColorSet();
    this->search_color = copy_color_set.GetSearchColor();
    this->score = copy_color_set.score;
    this->num_color = copy_color_set.num_color;
}

//基本initの初期化のみで使う
void ColorSet::InitColorSet(vector<ColorClass> copy_color_set) {
    this->S = copy_color_set;
}

void ColorSet::MoveVertexColor(int vertex,int after_color) {
    //どこかの色に属しているなら、そこから消す
    if( search_color.find(vertex) != search_color.end() ) {
        int pre_color = search_color[vertex];
        int pre_size = S[pre_color].vertexes.size();
        S[pre_color].vertexes.erase(vertex);
    }
    this->search_color[vertex] = after_color; //追加も入れ替えも両対応(map最高!)
    this->S[after_color].vertexes.insert(vertex);
}

void ColorSet::ShowColor(int pre_color,int vertex) {
    for(int t : S[pre_color].vertexes) {
        cout << t << " ";
    }
    cout << endl;
    cout << "vetex" << vertex << " color" << pre_color << endl;
}

//密度が大きいグラフ順に小さい重みを割り当てる
void ColorSet::SortColorWeight() {
    vector<long long> color_weight;
    this->search_color.clear();
    for( int i=0; i < S.size(); i++) {
        color_weight.push_back( S[i].weight );
    }
    sort(color_weight.begin(),color_weight.end());
    GreaterSort();
    for( int i=0; i < S.size(); i++ ) {
        this->S[i].weight = color_weight[i];
    }
}