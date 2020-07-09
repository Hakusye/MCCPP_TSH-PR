#include <iostream>
#include <fstream>
#include <utility>
#include <random>
#include "TSHwithPR.hpp"
#include "util.hpp"
using namespace std;

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
//テストクリア(テストケース... _RemoveSetTest)
template <typename T>
set<T> TSHwithPR::RemoveSet2Set(set<T> main_set,const set<T> remove_set) {
    for ( T remove_element : remove_set) {
        if( main_set.find(remove_element) != main_set.end() ) main_set.erase( remove_element );
    }
    return main_set;
}
//まだ
//Fasible solutionに修正するやつ。
void TSHwithPR::LocalSearch::CriticalOneMoveNeighborhood() {

}
//バグ発見 <- 対処済み
//多分完璧
void TSHwithPR::LocalSearch::ReassignLargestCardinality(double RF = 0.5) {
    set<int> uncolord_vertexes; 
    for(int i = 0; i < graph.size(); i++) {
        uncolord_vertexes.insert( i );
    }
    while( graph.size() * RF > current_color.search_color.size() ) {
        current_color.num_color++;//使用した色の数。割り当てるのは-1した値
        //塗られていない頂点集合の部分グラフでもっとも次数の大きいものを探す
        auto [ max_vertex, max_adjacent_set ] = LargestAdjacentVertexInSet(uncolord_vertexes,uncolord_vertexes);
        // 選んだ頂点を今操作している色に変更
        current_color = MoveVertexColor(current_color,max_vertex,current_color.num_color-1);
        // 初回のUとmax_adjacent_setが同等。V'からUの要素を抜く必要がある
        set<int> Uset = max_adjacent_set;
        uncolord_vertexes = RemoveSet2Set(uncolord_vertexes,Uset);
        uncolord_vertexes.erase(max_vertex);
        while( uncolord_vertexes.size() != 0 ) {
            //uncolord_setの中でUともっとも隣接している頂点を選択
            auto [ max_vertex, max_adjacent_set ] = LargestAdjacentVertexInSet(uncolord_vertexes,Uset);
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
    //O(n)にできそうだけど、わかりやすさからO(nlogn)を選択
    //残りは今ある色からランダムに決める
    std::mt19937 mt{ std::random_device{}() };
    mt.seed(4649);
    std::uniform_int_distribution<int> dist(0, current_color.num_color-1);

    for (int vertex = 0; vertex < graph.size(); vertex++) {
        map<int,int> search_color = current_color.search_color;
        if( search_color.find(vertex) != search_color.end() ) continue;
        current_color = MoveVertexColor(current_color,vertex,dist(mt));
    }
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

void TSHwithPR::EliteSetUpdate::PriorHighScore() {

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
    current_color.score = EvalFunction(current_color);
}