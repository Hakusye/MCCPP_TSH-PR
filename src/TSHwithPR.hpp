#pragma once
#include "util.hpp"
#include "MCCPP.hpp"

class TSHwithPR : public MCCPP {
  private:
    static const int elite_max_size = 5;
    static std::set<VertexMove> tabu_list;
    static std::vector<ColorSet> elite_set;
    
    //実装上、あると便利な関数達
    static ColorSet SortColorWeight(ColorSet color_set); //使用数の多い順にしたい時につかう　
    template <typename T>
    static std::set<T> RemoveSet2Set(std::set<T> main_set,const std::set<T> remove_set);
    template <typename T>
    static std::set<T> IntersectionSet2Set(const std::set<T> left_set,const std::set<T> right_set);

  public:
    TSHwithPR();
    ~TSHwithPR();
    // メイン探索
    class Greedy {
      public:
        static ColorSet CriticalOneMoveNeighborhood(ColorSet target_color_set);         // feasible search
        static void ReassignLargestCardinality(double RF); // solution_improvement
    };
    class LocalSearch {
      public:
        static ColorSet Reassign2SmallerOne(ColorSet color_set);  
    };
    class PathRelinkng {
      public:
        static std::pair<ColorSet,std::set<int>> CalcMoveDistance(ColorSet initial_S,ColorSet guiding_S);
        static ColorSet PathRelinking(ColorSet initial_S,ColorSet guiding_S,ColorSet goal_S,std::set<int> move_vertex);
    };
    class Perturbation {
      public:
        static ColorSet SetRandomColor(ColorSet target_color_set,int max_perturbation);
        int max_perturbation;
    };
    class EliteSetUpdate {
      public:
        static void PriorHighScore(ColorSet target_color_set);
        static bool EliteSetHaveElement(std::vector<ColorSet> target_elite,ColorSet target_color_set);
    };
    // その他機能
    static void Run();
    static ColorSet Result();
    //test関数。あとで消す
    static void _LargestAdjacentTest();
    static void _RemoveSet2SetTest();
    static void _MoveVertexColorTest();
    static void _ReassignTest();
    static void _Reassign2SmallerOneTest();
    static void _PathRelinkingTest();
    static void _EliteSetUpdateTest();
};