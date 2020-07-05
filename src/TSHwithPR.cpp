#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include "TSHwithPR.hpp"
#include "struct.hpp"
using namespace std;

bool operator< (const ColorSet &c1, const ColorSet &c2) { return c1.score > c2.score; }
//staticに実体をもたせた
int TSHwithPR::num_edges;
vector<vector<int>> TSHwithPR::graph;
ColorSet TSHwithPR::init_color; //初期の重みと色を記録,score,num_colorはnull
priority_queue<
    ColorSet,
    vector<ColorSet>,
    greater<ColorSet>> TSHwithPR::elite_set;
ColorSet TSHwithPR::current_color;

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
//まだ
TSHwithPR::TSHwithPR() {

};

TSHwithPR::~TSHwithPR() {

};


//テスト済
void TSHwithPR::_ShowGraph() {
    for(int i=0;i<init_color.S.size();i++){
        cout << init_color.S[i].weight << " ";
    }
    cout << endl;
    for(int i=0;i<graph.size();i++) {
        cout << "vertex" << i << ":";
        for(int j=0;j<graph[i].size();j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

void TSHwithPR::_ShowColorSet(ColorSet color_set) {
    cout << "score:" << color_set.score  << endl;
    for(ColorClass color_class : color_set.S) {
        if(color_class.vertexes.size() == 0) continue;
        for(int vertex : color_class.vertexes) { 
            cout << vertex << " ";
        }
        cout << endl;
    }
}

void TSHwithPR::InputGraph() {
    string filename;
    ifstream inputfile;
    string line;
    bool ok;
    do {
        ok = true;
        cout << "グラフのファイル名を入力してください" << endl;
        cin >> filename;
        inputfile.open(filename);
        if(inputfile.fail()) {
            cout << "Error:ファイルにアクセス出来ません" << endl;
            ok = false;
        }
    } while(!ok);

    while(getline(inputfile,line)) {
        vector<string> words = split(line, ' ');
        //入力データの形式に合わせる
        if(words[0] == "c") continue;
        else if(words[0] == "p") {
            graph.resize(stoi(words[2])); //余剰あってもいいかも
            num_edges = stoi(words[3]);
        }else if(words[0] == "e") {
            int a = stoi(words[1])-1; //頂点を0からにした
            int b = stoi(words[2])-1;
            graph[a].push_back(b);
            graph[b].push_back(a);
        }
    }
}

void TSHwithPR::InputColorSet() {
    string filename;
    ifstream inputfile;
    string line;
    bool ok;
    do {
        ok = true;
        cout << "カラークラスのファイル名を入力してください" << endl;
        cin >> filename;
        inputfile.open(filename);
        if(inputfile.fail()) {
            cout << "Error:ファイルにアクセス出来ません" << endl;
            ok = false;
        }
    }while(!ok);

    while(getline(inputfile,line)) {
        ColorClass C;
        C.weight = stod(line);
        init_color.S.push_back(C);
    }
    current_color = init_color;
    cout << "ok" << endl;
}
//まだ
void TSHwithPR::EliteSetUpdate() {

}

void TSHwithPR::Run() {

}

void TSHwithPR::ShowResult() {

}

void TSHwithPR::SavePath() {

}

void TSHwithPR::EvalFunction() {

}

void TSHwithPR::LocalSearch::CriticalOneMoveNeighborhood() {

}
//current_colorの初期解を作る
//動いた（あってるかわからない）。とりあえずおｋとする。(どうせ並び替える)
void TSHwithPR::LocalSearch::ReassignLargestCardinality(double RF = 0.5) {
    // num_adjacent , vertex numver
    set<int> uncolord_vertexes; 
    for(int i = 0; i < graph.size(); i++) {
        uncolord_vertexes.insert( i );
    }
    while( graph.size() * RF >= current_color.search_color.size() ) {
        current_color.num_color++;//使用した色の数。割り当てるのは-1した値
        //塗られていない頂点集合の部分グラフでもっとも次数の大きいものを探す
        auto [ max_vertex, max_adjacent_set ]= LargestAdjacentVertexInSet(uncolord_vertexes,uncolord_vertexes);
        // 選んだ頂点を今操作している色に変更
        current_color = MoveVertexColor(current_color,max_vertex,current_color.num_color-1);
        // 初回のUとmax_adjacent_setが同等。V'からUの要素を抜く必要がある
        set<int> Uset = max_adjacent_set;
        uncolord_vertexes = RemoveSet2Set(uncolord_vertexes,Uset);
        uncolord_vertexes.erase(max_vertex);
        while( uncolord_vertexes.size() != 0 ) {
            cout << uncolord_vertexes.size() << endl;
            //uncolord_setの中でUともっとも隣接している頂点を選択
            auto [ max_vertex, max_adjacent_set  ]= LargestAdjacentVertexInSet(uncolord_vertexes,Uset);
            current_color = MoveVertexColor(current_color,max_vertex,current_color.num_color-1);
            //V'からUの要素を抜く,
            //U と V' adjacent to vの足し合わせを表現
            set<int> U_insert_set = AdjacentColorSet(uncolord_vertexes,max_vertex);
            uncolord_vertexes = RemoveSet2Set(uncolord_vertexes,U_insert_set);
            uncolord_vertexes.erase(max_vertex);
            Uset.insert( U_insert_set.begin(),U_insert_set.end() );
        }
        uncolord_vertexes = Uset;
    }
}
//テストクリア(テストケース..._LargestAdjacentTest)
pair<int, set<int>> TSHwithPR::LargestAdjacentVertexInSet(const set<int> candidate_set,const set<int> graph_sub_set) {
    set<int> max_adjacent_set = {}; int max_vertex = -1;
    // uncolord_setの中でUともっとも隣接している頂点を選択
    for(int vertex : candidate_set) {
        set<int> tmp_adjacent_set = AdjacentColorSet(graph_sub_set, vertex);
        if(max_adjacent_set.size() < tmp_adjacent_set.size()) {
            max_adjacent_set = tmp_adjacent_set;
            max_vertex = vertex;
        }
    }
    return {max_vertex , max_adjacent_set};
}
//テストクリア(LargestAdjacentVErtexInSetに内包しているため)
set<int> TSHwithPR::AdjacentColorSet(set<int> vertexes_set,int vertex) {
    vector<int> g_adjasent_set = graph[vertex];//頂点vertexの隣合う頂点はグラフから確認できる
    set<int> ans_set;
    for (int vertex : g_adjasent_set) {
        if(vertexes_set.find(vertex) != vertexes_set.end()) ans_set.insert(vertex);
    }
    return ans_set;
}
//テストクリア(テストケース... _RemoveSetTest)
template <typename T> //あらゆる集合の型に対してremoveできるようにした
set<T> TSHwithPR::RemoveSet2Set(set<T> main_set,const set<T> remove_set) {
    for ( T remove_element : remove_set) {
        if( main_set.find(remove_element) != main_set.end() ) main_set.erase( remove_element );
    }
    return main_set;
}
//テストクリア(テストケース... _MoveVertexColorTest)
ColorSet TSHwithPR::MoveVertexColor(ColorSet target_set,int pre_vertex,int after_color) {
    //どこかの色に属しているなら、そこから消す
    auto tmp = target_set.search_color.find(pre_vertex);
    if( tmp != target_set.search_color.end() ) {
        int pre_color = target_set.search_color[pre_vertex];
        target_set.S[pre_color].vertexes.erase(pre_vertex);
    }
    target_set.search_color[pre_vertex] = after_color; //追加も入れ替えも両対応(map最高!)
    target_set.S[after_color].vertexes.insert(pre_vertex);
    return target_set;
}
//まだ
void TSHwithPR::PathRelinkng::CalcMoveDistance() {

}

void TSHwithPR::PathRelinkng::PathRelinking() {

}

void TSHwithPR::Perturbation::SetRandomColor() { 

}

TSHwithPR::Perturbation::Perturbation(int perturbation) { 
    max_perturbation = perturbation;
}

//テストケース
void TSHwithPR::_LargestAdjacentTest() {
    set<int> uncolord_vertexes; 
    for(int i = 0; i < graph.size(); i++) {
        uncolord_vertexes.insert( i );
    }
    auto [ max_vertex, max_adjacent_set ] = LargestAdjacentVertexInSet(uncolord_vertexes,uncolord_vertexes);
    cout << max_vertex << endl;
    for(int vertex : max_adjacent_set) {
        cout << vertex << " ";
    }
    cout << endl;
}

void TSHwithPR::_RemoveSet2SetTest() {
    set<int> a = {1,2,3,4,5,6,7,8,9};
    set<int> b = {1,3,5,7,9};
    set<int> ans = RemoveSet2Set(a,b);
    for(int vertex : ans){
        cout << vertex << " ";
    }
    cout << endl;
}

void TSHwithPR::_MoveVertexColorTest() {
    ColorSet cs;
    cs.score = 100;
    cs.S.resize(10);
    for(int i=0;i<20;i++) {
        cs = MoveVertexColor( cs, i, i%5 );
    }
    _ShowColorSet(cs);
    for(int i=0;i<5;i++) { 
        cs = MoveVertexColor( cs , i , 2 );
    }
    _ShowColorSet(cs);
}

void TSHwithPR::_ReassignTest() {
    LocalSearch::ReassignLargestCardinality();
    _ShowColorSet(current_color);
}