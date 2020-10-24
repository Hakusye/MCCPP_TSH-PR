#pragma once
#include "util.hpp"
//基底クラス。Input,OutPutや評価関数,移動系など
class MCCPP {
  protected:
    static std::mt19937 mt;
    static std::random_device rd;
    const static int M = 1e5; //切り捨て対策
    const static int INF = 1e9+7;
    static int num_edges;
    static std::vector<std::vector<int>> graph;
    static ColorSet init_color; //初期の重みと色を記録,score,num_colorはnull
    static long long DiffEvalFunction(const ColorSet color_set,int change_vertex,int after_color);
    //新しい方が計算時間がすくない(O(nlogn) -> O(n)),バグが見つかり次第戻す
    static std::set<int> AdjacentColorSet(std::set<int> vertexes_set,int vertex);
    static std::set<int> OldAdjacentColorSet(std::set<int> vertexes_set,int vertex);
    static ColorSet MoveVertexColor(ColorSet target_set,int pre_vertex,int after_color );
    static std::pair<int, std::set<int>> LargestAdjacentVertexInSet(const std::set<int> candidate_set,const std::set<int> graph_sub_set);
    static std::set<int> IntersectionSet2Set(const std::set<int> left_set,const std::set<int> right_set);
    static std::set<int> RemoveSet2Set(std::set<int> main_set,const std::set<int> remove_set);

  public:
    static ColorSet current_color; //testで調べるためにpublic.あとでprotected
    static ColorSet reset_color;
    static long long EvalFunction(const ColorSet color_set); //あとでprotexted
    MCCPP();
    ~MCCPP();
    static void Run();
    static void ShowResult();
    static void SavePath();
    static void InputGraph();
    static void InputGraph(std::string filename);
    static void InitGraph(std::ifstream& inputfile);
    static void InputColorSet();
    static void InputColorSet(std::string filename);
    static void InitColorSet(std::ifstream& inputfile);
    static void _ShowGraph();
    static void _ShowColorSet(ColorSet color_set);
};