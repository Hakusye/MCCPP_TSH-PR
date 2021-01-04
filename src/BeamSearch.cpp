#include "../include/TSHwithPR.hpp"
using namespace std;

vector<map< string ,ColorSet >> TSHwithPR::PathRelinking::BeamSearch::DP(2);   
set< pair<long long ,string >> TSHwithPR::PathRelinking::BeamSearch::sort_score_DP;
ColorSet TSHwithPR::PathRelinking::BeamSearch::optimal_colorset;

//PathRelinkingとしての最終的な出力
ColorSet TSHwithPR::PathRelinking::BeamSearch::Output() {
    return optimal_colorset;
}; 

//ビームサーチ全体の操作
void TSHwithPR::PathRelinking::BeamSearch::NodeSearch(const ColorSet &initial_S,const ColorSet &goal_S,const set<int> &move_vertexes) {
    InitBeamSearch(initial_S);
    string used_check = "";
    string clear_check = "";
    for(int i=0 ;i < move_vertexes.size(); i++) {
        used_check.append("0");
        clear_check.append("1");
    }
    for( int vertex : move_vertexes ) {
        UpdateDP( initial_S, vertex, goal_S.GetSearchColor()[vertex] , 0, used_check ,move_vertexes);
    }
    //cout << "init end" << endl;
    int count = 1; //隅奇を確認するやつDP[count%2]で使う
    while( DP[0].count(clear_check) == 0 && DP[1].count(clear_check) == 0 ) {
        for( auto [ moving_bit,colorset ] : DP[count%2] ) {
            for( int shift=0;shift < moving_bit.size(); shift++ ) {
                if( moving_bit[shift] == '1') continue; // 色変後なら飛ばす
                string string_check = moving_bit;
                string_check[shift] = '1';
                if( DP[(count+1)%2].count(string_check) != 0) continue; // 既に調べていたら飛ばす
                set<int>::iterator itr = move_vertexes.begin();
                //itr += shift;をしたい。setはランダム・アクセス不可よりループで処理
                for( int i=0; i< shift; i++ ) itr++;
                UpdateDP( colorset, *itr, goal_S.GetSearchColor()[*itr], (count+1)%2, moving_bit, move_vertexes );
            }
        }
        //このタイミングで最も良い結果を記録(仮実装)
        pair<long long,string> optimal_score = *(sort_score_DP.begin());
        //探索し終わった深さの最も良い評価がoptimal_colorsetよりも良い評価なら更新
        if( optimal_score.first < optimal_colorset.score ) {
            optimal_colorset = DP[(count+1)%2][optimal_score.second];
        }
        DP[count%2].clear(); // ややこしくなるので使用済みはリセット
        while( DP[(count+1)%2].size() > KEEP_NUM ) {
           set< pair<long long ,string > >::iterator itr = sort_score_DP.end(); itr--;
            DP[(count+1)%2].erase((*itr).second);
            sort_score_DP.erase(itr);
        }
        count++; // DPの配列を変更
        sort_score_DP.clear();
    }
};

//枝切りの条件もここで示す(初めは同じノードの上位10個くらい保持する感じ?)
//UpdateDPに枝切りをまとめて書いた
void TSHwithPR::PathRelinking::BeamSearch::BranchCut() {

};
//未テスト
void TSHwithPR::PathRelinking::BeamSearch::UpdateDP(ColorSet original_set,int change_vertex, int change_color,int num_dp,string used_check ,const set<int> &move_vertexes) {
    long long next_score = DiffEvalFunction(original_set,change_vertex,change_color);
    original_set.MoveVertexColor(change_vertex,change_color);
    original_set.score = next_score;
    //cout << distance( move_vertexes.begin(), move_vertexes.find( change_vertex )) << endl;
    used_check[ distance( move_vertexes.begin(), move_vertexes.find( change_vertex )) ] = '1'; // 色を付けた判定
    //cout << DP.size() << endl;
    DP[num_dp].insert(make_pair(used_check,original_set)); // 一旦変えた先を代入
    sort_score_DP.insert(make_pair( next_score, used_check ));
    //上位の評価からはずれたデータを削除
};

void TSHwithPR::PathRelinking::BeamSearch::InitBeamSearch(const ColorSet &colorset) {
    optimal_colorset = colorset;
    DP[0].clear();
    DP[1].clear();
};