#pragma once
#include <queue>
#include <vector>
#include <utility>
#include "struct.hpp"
//基底クラス。Input,OutPutや評価関数,移動系など
class MCCPP {
  protected:
    static ColorSet current_color;
    const static int M = 1e5 + 7;
    static int num_edges;
    static std::vector<std::vector<int>> graph;
    static ColorSet init_color; //初期の重みと色を記録,score,num_colorはnull
    static double EvalFunction(const ColorSet color_set);
    static std::set<int> AdjacentColorSet(std::set<int> vertexes_set,int vertex);

  public:
    MCCPP();
    ~MCCPP();
    static void Run();
    static void ShowResult();
    static void SavePath();
    static void InputGraph();
    static void InputColorSet();
    static void _ShowGraph();
    static void _ShowColorSet(ColorSet color_set);
};