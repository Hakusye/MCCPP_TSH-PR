#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include "../include/util.hpp"
#include "../include/MCCPP.hpp"
#include "../include/FullSearch.hpp"

using namespace std;
//完成
const double FullSearch::INF = 100000.0;
FullSearch::FullSearch() {

};

FullSearch::~FullSearch() {

};
//若干やっかいなので治すのはとりあえず保留
/*
void FullSearch::Calc() {
    //初期解すべて0
    current_color.score = INF;
    init_color.score = 0; //DFSで増えていく方式
    for( int i=0; i < graph.size(); i++) {
        DFS4FullSerch(init_color,0,i);
    }
}
//最大の値をそのままcurrent_colorにいれる

void FullSearch::DFS4FullSerch( ColorSet color_set ,int target_vertex,int color) {
    //色を追加したことによる変化
    color_set.S[color].vertexes.insert(target_vertex);
    color_set.search_color[target_vertex] = color;
    color_set.score += color_set.S[color].weight;
    if (!AdjacentColorSet(color_set.S[color].vertexes,target_vertex).empty()) {
        return; //隣接点の色がかぶってるならおしまい
    }
    //色塗り終わったタイミングで変換するか決める
    if (target_vertex+1 == graph.size()){
        if(current_color.score <= color_set.score) return;
        current_color = color_set;
        return;
    }
    for( int i = 0; i < graph.size(); i++) {
        DFS4FullSerch(color_set, target_vertex+1, i);
    }
}
*/
