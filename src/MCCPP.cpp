#include <iostream>
#include <fstream>
#include "TSHwithPR.hpp"
#include "util.hpp"
using namespace std;

ColorSet MCCPP::current_color;
//まだ
MCCPP::MCCPP() {

}

MCCPP::~MCCPP() {

}
//テストクリア(ほぼ確実に正しい)
double MCCPP::EvalFunction(const ColorSet color_set) {
    double sum = 0;
    for(ColorClass target : color_set.S) {
        sum += target.weight * target.vertexes.size();
        for(int vertex : target.vertexes) { //その色の中で隣接があるなら数分M足す
            set<int> neighbor_vertexes = AdjacentColorSet(target.vertexes,vertex);
            sum +=  neighbor_vertexes.size() * M / 2.0;
        }
    }
    return sum; 
}

void MCCPP::_ShowGraph() {
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

void MCCPP::_ShowColorSet(ColorSet color_set) {
    cout << "score:" << color_set.score  << endl;
    for(ColorClass color_class : color_set.S) {
        if(color_class.vertexes.size() == 0) continue;
        for(int vertex : color_class.vertexes) { 
            cout << vertex << " ";
        }
        cout << endl;
    }
}

void MCCPP::InputGraph() {
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

void MCCPP::InputColorSet() {
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
//テストクリア(テストケース..._LargestAdjacentTest)
pair<int, set<int>> MCCPP::LargestAdjacentVertexInSet(const set<int> candidate_set,const set<int> graph_sub_set) {
    set<int> max_adjacent_set = {};
    int max_vertex = -1; int max_adjacent = -1;
    // uncolord_setの中でUともっとも隣接している頂点を選択
    for(int vertex : candidate_set) {
        set<int> tmp_adjacent_set = AdjacentColorSet(graph_sub_set, vertex);
        int tmp_size = tmp_adjacent_set.size();
        if( max_adjacent < tmp_size ) {
            max_adjacent_set = tmp_adjacent_set;
            max_vertex = vertex;
            max_adjacent = tmp_adjacent_set.size();
        }
    }
    return {max_vertex , max_adjacent_set};
}

set<int> MCCPP::AdjacentColorSet(set<int> vertexes_set,int target_vertex) {
    vector<int> g_adjasent_set = graph[target_vertex];//頂点vertexの隣合う頂点はグラフから確認できる
    set<int> ans_set;
    for (int vertex : g_adjasent_set) {
        if( vertex == target_vertex) continue;
        if(vertexes_set.find(vertex) != vertexes_set.end()) ans_set.insert(vertex);
    }
    return ans_set;
}
//テストクリア(テストケース... _MoveVertexColorTest)(Diffを追加してからは未テスト)
//動作後の整合性を整える(評価関数変えたくない時あるのでDiffEvalは別でやる)
ColorSet MCCPP::MoveVertexColor(ColorSet target_set,int pre_vertex,int after_color) {
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
//未テスト（なにかと一緒にテストする）
double MCCPP::DiffEvalFunction(const ColorSet color_set,int change_vertex,int after_color) {
    double ans = color_set.score;
    int pre_color = color_set.search_color.at(change_vertex);
    
    ans -= color_set.S[pre_color].weight;
    set<int> adjacent_color = AdjacentColorSet(color_set.S[pre_color].vertexes,change_vertex);
    ans -= adjacent_color.size() * M; // 隣接頂点が同じだった数だけ引く

    ans += color_set.S[after_color].weight;
    adjacent_color = AdjacentColorSet(color_set.S[after_color].vertexes,change_vertex);
    ans += adjacent_color.size() * M; // 隣接頂点の色が同じになった数だけ増やす

    return ans;
}
//まだ
void MCCPP::Run() {

}

void MCCPP::ShowResult() {

}

void MCCPP::SavePath() {

}