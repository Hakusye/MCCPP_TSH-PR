#pragma once
#include <queue>
#include <utility>
#include "util.hpp"
//基底クラス。Input,OutPutや評価関数,移動系など
class MCCPP {
  protected:
    const static int M = 1e5 + 7;
    static int num_edges;
    static std::vector<std::vector<int>> graph;
    static ColorSet init_color; //初期の重みと色を記録,score,num_colorはnull
    static double DiffEvalFunction(const ColorSet color_set,int change_vertex,int after_color);
    static std::set<int> AdjacentColorSet(std::set<int> vertexes_set,int vertex);
    static ColorSet MoveVertexColor(ColorSet target_set,int pre_vertex,int after_color );
    static std::pair<int, std::set<int>> LargestAdjacentVertexInSet(const std::set<int> candidate_set,const std::set<int> graph_sub_set);

  public:
    static ColorSet current_color; //testで調べるためにpublic.あとでprotected
    static double EvalFunction(const ColorSet color_set); //あとでprotexted
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