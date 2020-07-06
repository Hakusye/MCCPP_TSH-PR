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
int MCCPP::num_edges;
vector<vector<int>> MCCPP::graph;
ColorSet MCCPP::init_color; //初期の重みと色を記録,score,num_colorはnull
priority_queue<
    ColorSet,
    vector<ColorSet>,
    greater<ColorSet>> TSHwithPR::elite_set;

//まだ
TSHwithPR::TSHwithPR() {

};

TSHwithPR::~TSHwithPR() {

};
//まだ
void TSHwithPR::EliteSetUpdate() {

}
//Fasible solutionに修正するやつ。
void TSHwithPR::LocalSearch::CriticalOneMoveNeighborhood() {

}
//バグ発見(テストケース_ReassignTest) uncolord_vertexesの個数が途中で変わらなくなる
//残りの部分random設置もつくってない
void TSHwithPR::LocalSearch::ReassignLargestCardinality(double RF = 0.5) {
    // num_adjacent , vertex numver
    set<int> uncolord_vertexes; 
    for(int i = 0; i < graph.size(); i++) {
        uncolord_vertexes.insert( i );
    }
    while( graph.size() * RF >= current_color.search_color.size() ) {
        cerr << graph.size() * RF <<" " << current_color.search_color.size() << endl;

        current_color.num_color++;//使用した色の数。割り当てるのは-1した値
        //塗られていない頂点集合の部分グラフでもっとも次数の大きいものを探す
        auto [ max_vertex, max_adjacent_set ]= LargestAdjacentVertexInSet(uncolord_vertexes,uncolord_vertexes);
        // 選んだ頂点を今操作している色に変更
        current_color = MoveVertexColor(current_color,max_vertex,current_color.num_color-1);
        // 初回のUとmax_adjacent_setが同等。V'からUの要素を抜く必要がある
        set<int> Uset = max_adjacent_set;
        uncolord_vertexes = RemoveSet2Set(uncolord_vertexes,Uset);
        uncolord_vertexes.erase(max_vertex);
        while( uncolord_vertexes.size() != 0 && graph.size() * RF >= current_color.search_color.size()) {
            //cerr << current_color.search_color.size() << endl;
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
    LocalSearch::ReassignLargestCardinality(0.7);
    current_color.score = EvalFunction(current_color);
    _ShowColorSet(current_color);
}