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
    //test用。消しても問題ない
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
    static std::tuple<std::set<int>,bool,bool,int,int> CriticalOneMoveNeighborhood(const ColorSet &target_color_set);         // feasible search
    static void ReassignLargestCardinality(double RF); // solution_improvement
};

class TSHwithPR::LocalSearch {
  public:
    static ColorSet Reassign2SmallerOne(ColorSet color_set);  
};

class TSHwithPR::PathRelinking {
  public:
    static std::pair<ColorSet,std::set<int>> CalcMoveDistance(const ColorSet &initial_S,const ColorSet &guiding_S);
    static ColorSet CalcPathRelinking(const ColorSet &initial_S,const ColorSet &goal_S,std::set<int> move_vertex);
    class BeamSearch; //内部クラスの内部クラス
    class DAG;
}; 

class TSHwithPR::Perturbation {
  public:
    static ColorSet SetRandomColor(ColorSet target_color_set,int max_perturbation);
    int max_perturbation;
};

class TSHwithPR::EliteSetUpdate {
  public:
    static void PriorHighScore(const ColorSet &target_color_set);
    static bool EliteSetHaveElement(const std::vector<ColorSet> &target_elite,const ColorSet &target_color_set);
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
    static void NodeSearch(const ColorSet &initial_set,const ColorSet &goal_S,const std::set<int> &move_vertexes);
    static void BranchCut();
    static void UpdateDP(ColorSet original_set,int change_vertex, int change_color,int num_dp, std::string used_check ,const std::set<int> &move_vertexes);
    static void InitBeamSearch(const ColorSet &colorset);
    //evalはMCCPPのを一旦使用することにする
};
//色分けグラフを有向パスに書き直す処理とデータを保持する枠が必要
class TSHwithPR::PathRelinking::DAG {
  private:
    static std::vector<std::vector<int>> dag_graph;
    static std::vector<std::vector<int>> reverse_dag_graph; //逆向きの有向グラフ.逆向きに参照したい時に使う
    static std::vector<std::vector<int>> dag2original_vertexes;//dagの頂点から元の有向グラフ時の頂点番号を引き出すやつ
    static std::vector<std::set<int>> transitive_closure;
    static std::vector<int> dag_weight;

  public:
  //便宜上、move_vertexes順に0,1,2,..方式で頂点を保存する。move_vertexes[数字]で、元の頂点番号を復元できる
    static std::vector<std::vector<int>> MakeUndirectedGraph( const std::vector<int> &move_vertexes );
    static std::vector<std::vector<int>> MakeDirectedGraph(const std::vector<int> &move_vertexes,const ColorSet &init_set,const ColorSet &guiding_set);
    struct StronglyConnectedComponents;
    static std::vector<int> MakeDagWeight(const StronglyConnectedComponents &SCC,const std::vector<int> &move_vertexes,const ColorSet &init_set,const ColorSet &guiding_set); //その頂点集合の色を変更した場合のスコア増減の記録
    static std::vector<std::vector<int>> MakeDag2OriginalVertexes(const StronglyConnectedComponents &SCC,const std::vector<int> &move_vertexes);
    static void MakeReverseDagGraph();
    static void MakeTransitiveClosure(); //ワーシャルフロイドで書く
    static void Build(const std::vector<int> &move_vertexes,const ColorSet &init_set,const ColorSet &guiding_set);
    // 色変するかしないかのbool。move_vertexesの配列番号順に返す。
    static std::vector<bool> DagGreedy();
    //色変する頂点集合を元のグラフの頂点番号で出力
    static std::vector<int> CalcChangesVertexes( const std::vector<int> &move_vertexes );
    //推移閉包も作る。(それ自体が良い結果は産まないので(DagGreedyに負ける)本論のみに記載するかも)
    //test用
    void _test1();
    void _test2();
};
