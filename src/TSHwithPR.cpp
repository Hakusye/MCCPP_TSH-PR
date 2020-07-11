#include "TSHwithPR.hpp"
#include "util.hpp"
using namespace std;

//staticに実体をもたせた
int MCCPP::num_edges;
vector<vector<int>> MCCPP::graph;
ColorSet MCCPP::init_color; //初期の重みと色を記録,score,num_colorはnull
priority_queue<
    ColorSet,
    vector<ColorSet>,
    greater<ColorSet>> TSHwithPR::elite_set;

//まだ
TSHwithPR::TSHwithPR() {

};

TSHwithPR::~TSHwithPR() {

};
//テストクリア(テストケース... _RemoveSetTest)
template <typename T>
set<T> TSHwithPR::RemoveSet2Set(set<T> main_set,const set<T> remove_set) {
    for ( T remove_element : remove_set) {
        if( main_set.find(remove_element) != main_set.end() ) main_set.erase( remove_element );
    }
    return main_set;
}
//テストクリア(テストケース... _Reassign2SmallerOneTest)
ColorSet TSHwithPR::SortColorWeight(ColorSet color_set) {
    vector<float> color_weight;
    for( int i=0; i < color_set.S.size(); i++) {
        color_weight.push_back( color_set.S[i].weight );
    }
    sort(color_set.S.begin() , color_set.S.end(), greater<ColorClass>() );
    for( int i=0; i < color_set.S.size(); i++ ) {
        color_set.S[i].weight = color_weight[i];
    }
    return color_set;
}
//テストクリア(テストケース.. _Reassigntest) ただし、tabulist追加してない
// 1回の実行につきひとつの衝突をなくす。1つ1つの動作はbast solutionを選ぶ
ColorSet TSHwithPR::Greedy::CriticalOneMoveNeighborhood(ColorSet target_color_set) {
    //衝突頂点ををループ(入力として与えておく?)
    //すべての衝突頂点に衝突を無くせる色に変える全探索をして一番よかったやつにする
    //まだtabulistを組み込んでいない
    ColorSet ans_color_set = target_color_set;
    //衝突セットつくる
    set<int> conflict_set;
    for (int vertex = 0; vertex < graph.size(); vertex++) {
        int v_color = target_color_set.search_color.at(vertex);
        set<int> adjacent_set = AdjacentColorSet(target_color_set.S[v_color].vertexes,vertex);
        if(!adjacent_set.empty()) conflict_set.insert(vertex);
    }
    if(conflict_set.empty()){
        cerr << "衝突してないです" << endl;
        return target_color_set;
    }
    for (int vertex : conflict_set) {
        ColorSet tmp_color_set = target_color_set;
        int current_color = target_color_set.search_color[vertex];
        float tmp_score;
        for ( int color = 0; color < target_color_set.num_color+1; color++) {
            if(color == current_color) continue;
            //次に変える予定の色と隣接していないものをえらぶ
            set<int> adjacent_set = AdjacentColorSet(target_color_set.S[color].vertexes,vertex);
            if(!adjacent_set.empty()) continue;
            tmp_score = DiffEvalFunction(tmp_color_set,vertex,color);
            if(ans_color_set.score > tmp_score) { //tabulist弾くのも書く必要ある
                ans_color_set = tmp_color_set;
                ans_color_set.score = tmp_score;
                if(color == target_color_set.num_color) ans_color_set.num_color++;
                ans_color_set = MoveVertexColor(ans_color_set,vertex,color);
            }
        }
    }
    return ans_color_set;
}
//バグ発見 <- 対処済み
//多分完璧(RF=1.0で完全な貪欲ができる)
void TSHwithPR::Greedy::ReassignLargestCardinality(double RF = 0.5) {
    set<int> uncolord_vertexes; 
    for(int i = 0; i < graph.size(); i++) {
        uncolord_vertexes.insert( i );
    }
    while( graph.size() * RF > current_color.search_color.size() ) {
        current_color.num_color++;//使用した色の数。割り当てるのは-1した値
        //塗られていない頂点集合の部分グラフでもっとも次数の大きいものを探す
        auto [ max_vertex, max_adjacent_set ] = LargestAdjacentVertexInSet(uncolord_vertexes,uncolord_vertexes);
        // 選んだ頂点を今操作している色に変更
        current_color = MoveVertexColor(current_color,max_vertex,current_color.num_color-1);
        // 初回のUとmax_adjacent_setが同等。V'からUの要素を抜く必要がある
        set<int> Uset = max_adjacent_set;
        uncolord_vertexes = RemoveSet2Set(uncolord_vertexes,Uset);
        uncolord_vertexes.erase(max_vertex);
        while( uncolord_vertexes.size() != 0 ) {
            //uncolord_setの中でUともっとも隣接している頂点を選択
            auto [ max_vertex, max_adjacent_set ] = LargestAdjacentVertexInSet(uncolord_vertexes,Uset);
            current_color = MoveVertexColor(current_color,max_vertex,current_color.num_color-1);
            //V'からUの要素を抜く,
            //U と V' adjacent to vの足し合わせを表現
            set<int> U_insert_set = AdjacentColorSet(uncolord_vertexes,max_vertex);
            uncolord_vertexes = RemoveSet2Set(uncolord_vertexes,U_insert_set);
            uncolord_vertexes.erase(max_vertex);
            Uset.insert( U_insert_set.begin(),U_insert_set.end() );
        }
        uncolord_vertexes = Uset;
    }
    //O(n)にできそうだけど、わかりやすさからO(nlogn)を選択
    //残りは今ある色からランダムに決める
    std::mt19937 mt{ std::random_device{}() };
    mt.seed(4649);
    std::uniform_int_distribution<int> dist(0, current_color.num_color-1);

    for (int vertex = 0; vertex < graph.size(); vertex++) {
        map<int,int> search_color = current_color.search_color;
        if( search_color.find(vertex) != search_color.end() ) continue;
        current_color = MoveVertexColor(current_color,vertex,dist(mt));
    }
}
//テストクリア(テストケース... _Reassign2SmallerOneTest)
ColorSet TSHwithPR::LocalSearch::Reassign2SmallerOne( ColorSet color_set ) {
    //使用数が大きい順に、重みが小さい順にする
    color_set = SortColorWeight(color_set);
    // 頂点数が多い順に回し、今選ばれているカラークラスより小さいやつ(なければそのサイズ)にいれる
    for (int target_color = 1; target_color < color_set.S.size(); target_color++) {
        set<int> target_vartex = color_set.S[target_color].vertexes;
        for(int vertex : target_vartex) { 
            for (int cacndidate_color = 0; cacndidate_color < target_color; cacndidate_color++) {
                //候補集合と今見ている頂点が隣接してるならスキップ
                set<int> adjacent_check = AdjacentColorSet(color_set.S[cacndidate_color].vertexes,vertex);
                if(!adjacent_check.empty()) continue;
                //更新。これより後ろみても意味なしなのでbreak
                color_set.score = DiffEvalFunction(color_set,vertex,cacndidate_color);
                color_set = MoveVertexColor(color_set,vertex,cacndidate_color);
                break;
            }
        } 
    }
    return color_set;
}

void TSHwithPR::PathRelinkng::CalcMoveDistance() {

}

void TSHwithPR::PathRelinkng::PathRelinking() {

}

void TSHwithPR::Perturbation::SetRandomColor() { 

}

TSHwithPR::Perturbation::Perturbation(int perturbation=0.3) { 
    max_perturbation = perturbation;
}

void TSHwithPR::EliteSetUpdate::PriorHighScore() {

}

//テストケース
void TSHwithPR::_LargestAdjacentTest() {
    set<int> uncolord_vertexes; 
    for(int i = 0; i < graph.size(); i++) {
        uncolord_vertexes.insert( i );
    }
    auto [ max_vertex, max_adjacent_set ] = LargestAdjacentVertexInSet(uncolord_vertexes,uncolord_vertexes);
    cout << max_vertex << endl;
    for(int vertex : max_adjacent_set) {
        cout << vertex << " ";
    }
    cout << endl;
}
void TSHwithPR::_RemoveSet2SetTest() {
    set<int> a = {1,2,3,4,5,6,7,8,9};
    set<int> b = {1,3,5,7,9};
    set<int> ans = RemoveSet2Set(a,b);
    for(int vertex : ans){
        cout << vertex << " ";
    }
    cout << endl;
}
void TSHwithPR::_MoveVertexColorTest() {
    ColorSet cs;
    cs.score = 100;
    cs.S.resize(10);
    for(int i=0;i<20;i++) {
        cs = MoveVertexColor( cs, i, i%5 );
    }
    _ShowColorSet(cs);
    for(int i=0;i<5;i++) { 
        cs = MoveVertexColor( cs , i , 2 );
    }
    _ShowColorSet(cs);
}
void TSHwithPR::_ReassignTest() {
    Greedy::ReassignLargestCardinality(0.5);
    current_color.score = EvalFunction(current_color);
    _ShowColorSet(current_color);
    while(current_color.score > 10000) { //Criticalを回す条件式は後で考える必要あり
        current_color = Greedy::CriticalOneMoveNeighborhood(current_color);
        _ShowColorSet(current_color);
    }
}
void TSHwithPR::_Reassign2SmallerOneTest() {
    _ReassignTest();
    current_color = LocalSearch::Reassign2SmallerOne(current_color);
}