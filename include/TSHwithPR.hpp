#pragma once
#include "MCCPP.hpp"

class TSHwithPR : public MCCPP {
  protected:
    static const int elite_max_size = 5;
    static std::set<VertexMove> tabu_list;
    static std::vector<ColorSet> elite_set;
    
    //実装上、あると便利な関数達
    static ColorSet SortColorWeight(ColorSet color_set); //使用数の多い順にしたい時につかう　

  public:
    TSHwithPR();
    ~TSHwithPR();
    // メイン探索
    class Greedy;
    class LocalSearch;
    class PathRelinking;
    class Perturbation;
    class EliteSetUpdate;
    // その他機能
    static void ResetData();
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

class TSHwithPR::Greedy {
  public:
    static std::tuple<std::set<int>,bool,ColorSet> CriticalOneMoveNeighborhood(ColorSet target_color_set);         // feasible search
    static void ReassignLargestCardinality(double RF); // solution_improvement
};

class TSHwithPR::LocalSearch {
  public:
    static ColorSet Reassign2SmallerOne(ColorSet color_set);  
};

class TSHwithPR::PathRelinking {
  public:
    static std::pair<ColorSet,std::set<int>> CalcMoveDistance(ColorSet initial_S,ColorSet guiding_S);
    static ColorSet CalcPathRelinking(ColorSet initial_S,ColorSet goal_S,std::set<int> move_vertex);
    class BeamSearch; //内部クラスの内部クラス
}; 

class TSHwithPR::Perturbation {
  public:
    static ColorSet SetRandomColor(ColorSet target_color_set,int max_perturbation);
    int max_perturbation;
};

class TSHwithPR::EliteSetUpdate {
  public:
    static void PriorHighScore(ColorSet target_color_set);
    static bool EliteSetHaveElement(std::vector<ColorSet> target_elite,ColorSet target_color_set);
};

class TSHwithPR::PathRelinking::BeamSearch {
  private:
    const static int BEAM_WIDTH = 5;
    const static int KEEP_NUM = 10;
    static std::vector< std::map < std::string ,ColorSet >> DP;
    static std::set< std::pair<long long ,std::string >> sort_score_DP;
    static ColorSet optimal_colorset;
  
    //この仕様(unsigned long long)だと距離64までしか計算できないため、stringで行う
  public:
    static ColorSet Output();
    static void NodeSearch(ColorSet initial_set,ColorSet goal_S,std::set<int> move_vertexes);
    static void BranchCut();
    static void UpdateDP(ColorSet original_set,int change_vertex, int change_color,int num_dp, std::string used_check ,const std::set<int> &move_vertexes);
    static void InitBeamSearch(const ColorSet &colorset);
    //evalはMCCPPのを一旦使用することにする
};
