#pragma once
#include <queue>
#include <utility>
#include "util.hpp"
#include "MCCPP.hpp"

class TSHwithPR : public MCCPP {
  private:
    static std::priority_queue<
        ColorSet, 
        std::vector<ColorSet>,
        std::greater<ColorSet>> elite_set;
    
    //実装上、あると便利な関数達
    static void MoveColorWeight(); //重みを移動させる時に使う
    template <typename T>
    static std::set<T> RemoveSet2Set(std::set<T> main_set,const std::set<T> remove_set);

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
    class EliteSetUpdate {
      public:
        void PriorHighScore();

    };
    // その他機能

    //test関数。あとで消す
    static void _LargestAdjacentTest();
    static void _RemoveSet2SetTest();
    static void _MoveVertexColorTest();
    static void _ReassignTest();
};