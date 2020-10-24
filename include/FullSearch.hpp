#pragma once
#include <queue>
#include <vector>
#include <utility>
#include "util.hpp"
#include "MCCPP.hpp"

//完成
//簡単なベンチマーク生成(最適解がわかっているグラフと重み)に使う。
class FullSearch : public MCCPP {
  private:
    // max 10頂点
    static const double INF;
  public:
    FullSearch();
    ~FullSearch();
    static void Calc(); //current_setに最適解を返す
    static void DFS4FullSerch(ColorSet color_set, int target_vertex,int color);
    
};