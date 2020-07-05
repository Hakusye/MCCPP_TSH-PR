#pragma once
#include <queue>
#include <vector>
#include <utility>
#include "struct.hpp"

//used priority_queue
bool operator<(const ColorSet &c1, const ColorSet &c2);

class TSHwithPR {
  private:
    const static int M = 1e4+7;
    static int num_edges;
    static std::vector<std::vector<int>> graph;
    static ColorSet init_color; //初期の重みと色を記録,score,num_colorはnull
    static std::priority_queue<
        ColorSet, 
        std::vector<ColorSet>,
        std::greater<ColorSet>> elite_set;
    static ColorSet current_color;
    
    //実装上、あると便利な関数達
    static void EvalFunction();
    static ColorSet MoveVertexColor(ColorSet target_set,int pre_vertex,int after_color );
    static void MoveColorWeight();
    static std::set<int> AdjacentColorSet(std::set<int> vertexes_set,int vertex);
    template <typename T>
    static std::set<T> RemoveSet2Set(std::set<T> main_set,const std::set<T> remove_set);
    static std::pair<int, std::set<int>> LargestAdjacentVertexInSet(const std::set<int> candidate_set,const std::set<int> graph_sub_set);

  public:
    TSHwithPR();
    ~TSHwithPR();
    // メイン探索
    class LocalSearch {
      public:
        static void CriticalOneMoveNeighborhood();         // feasible search
        static void ReassignLargestCardinality(double RF); // solution_improvement
    };
    class PathRelinkng {
      public:
        static void CalcMoveDistance();
        static void PathRelinking();
    };
    class Perturbation {
      public:
        Perturbation(int perturbation);
        static void SetRandomColor();
        std::vector<VertexMove> tabu_list;
        int max_perturbation;
    };
    static void EliteSetUpdate();
    // その他機能
    static void Run();
    static void ShowResult();
    static void SavePath();
    static void InputGraph();
    static void InputColorSet();
    static void _ShowGraph();
    static void _ShowColorSet(ColorSet color_set);

    //test関数。あとで消す
    static void _LargestAdjacentTest();
    static void _RemoveSet2SetTest();
    static void _MoveVertexColorTest();
    static void _ReassignTest();
};