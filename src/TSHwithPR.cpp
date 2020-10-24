#include "../include/TSHwithPR.hpp"
using namespace std;

//staticに実体をもたせた
int MCCPP::num_edges;
vector<vector<int>> MCCPP::graph;
ColorSet MCCPP::init_color; //初期の重みと色を記録,score,num_colorはnull
vector<ColorSet> TSHwithPR::elite_set;
set<VertexMove> TSHwithPR::tabu_list;
mt19937 MCCPP::mt;
random_device MCCPP::rd;

TSHwithPR::TSHwithPR() {
    current_color.num_color = 0;
};

TSHwithPR::~TSHwithPR() {

};

//テストクリア(テストケース.. _Reassigntest) ただし、tabulist追加してない
// 1回の実行につきひとつの衝突をなくす。1つ1つの動作はbast solutionを選ぶ
tuple<set<int>,bool,ColorSet> TSHwithPR::Greedy::CriticalOneMoveNeighborhood(ColorSet target_color_set) {
    //衝突頂点ををループ(入力として与えておく?)
    //すべての衝突頂点に衝突を無くせる色に変える全探索をして一番よかったやつにする
    //まだtabulistを組み込んでいない
    ColorSet ans_color_set = target_color_set;
    //衝突セットつくる
    set<int> conflict_set;
    vector<ColorClass> target_color_classes = target_color_set.GetColorSet();
    for (int vertex = 0; vertex < graph.size(); vertex++) {
        int v_color = target_color_set.GetSearchColor().at(vertex);
        set<int> adjacent_set = AdjacentColorSet(target_color_classes[v_color].vertexes,vertex);
        if(!adjacent_set.empty()) conflict_set.insert(vertex);
    }
    if(conflict_set.empty()) {
        return { conflict_set,false, target_color_set };
    }
    for (int vertex : conflict_set) {
        ColorSet tmp_color_set = target_color_set;
        int current_color = target_color_set.GetSearchColor()[vertex];
        long long tmp_score;
        for ( int color = 0; color <= target_color_set.num_color; color++) {
            if(color == current_color) continue;
            //次に変える予定の色と隣接していないものをえらぶ
            set<int> adjacent_set = AdjacentColorSet(target_color_set.GetColorSet()[color].vertexes,vertex);
            if(!adjacent_set.empty()) continue;
            tmp_score = DiffEvalFunction(tmp_color_set,vertex,color);
            if(ans_color_set.score > tmp_score) { //tabulist弾くのも書く必要ある
                ans_color_set = tmp_color_set;
                ans_color_set.score = tmp_score;
                if(color == target_color_set.num_color) ans_color_set.num_color++;
                ans_color_set.MoveVertexColor(vertex,color);
            }
        }
    }
    return { conflict_set,true, ans_color_set };
}
//バグ発見 <- 対処済み
//多分完璧(RF=1.0で完全な貪欲ができる)
void TSHwithPR::Greedy::ReassignLargestCardinality(double RF = 0.5) {
    set<int> uncolord_vertexes; 
    for(int i = 0; i < graph.size(); i++) {
        uncolord_vertexes.insert( i );
    }
    while( graph.size() * RF > current_color.GetSearchColor().size() ) {
        current_color.num_color++;//使用した色の数。割り当てるのは-1した値
        //塗られていない頂点集合の部分グラフでもっとも次数の大きいものを探す
        auto [ max_vertex, max_adjacent_set ] = LargestAdjacentVertexInSet(uncolord_vertexes,uncolord_vertexes);
        // 選んだ頂点を今操作している色に変更
        current_color.MoveVertexColor( max_vertex,current_color.num_color-1 );
        // 初回のUとmax_adjacent_setが同等。V'からUの要素を抜く必要がある
        set<int> Uset = max_adjacent_set;
        uncolord_vertexes = RemoveSet2Set(uncolord_vertexes,Uset);
        uncolord_vertexes.erase(max_vertex);
        while( uncolord_vertexes.size() != 0 ) {
            //uncolord_setの中でUともっとも隣接している頂点を選択
            auto [ max_vertex, max_adjacent_set ] = LargestAdjacentVertexInSet(uncolord_vertexes,Uset);
            current_color.MoveVertexColor(max_vertex,current_color.num_color-1);
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
    uniform_int_distribution<int> dist(0, current_color.num_color-1);

    for (int vertex = 0; vertex < graph.size(); vertex++) {
        map<int,int> search_color(current_color.GetSearchColor());
        if( search_color.find(vertex) != search_color.end() ) continue;
        current_color.MoveVertexColor(vertex,dist(mt));
    }
}
//テストクリア(テストケース... _Reassign2SmallerOneTest)
ColorSet TSHwithPR::LocalSearch::Reassign2SmallerOne( ColorSet color_set ) {
    //使用数が大きい順に、重みが小さい順にする
    color_set.SortColorWeight();
    color_set.score = EvalFunction(color_set);
    // 頂点数が多い順に回し、今選ばれているカラークラスより小さいやつ(なければそのサイズ)にいれる
    vector<ColorClass> color_classes = color_set.GetColorSet();
    for (int target_color = 1; target_color < color_classes.size(); target_color++) {
        set<int> target_vartex = color_classes[target_color].vertexes;
        for(int vertex : target_vartex) { 
            for (int cacndidate_color = 0; cacndidate_color < target_color+1; cacndidate_color++) {
                //候補集合と今見ている頂点が隣接してるならスキップ
                set<int> adjacent_check = AdjacentColorSet(color_classes[cacndidate_color].vertexes,vertex);
                if(!adjacent_check.empty()) continue;
                //更新。これより後ろみても意味なしなのでbreak
                color_set.score = DiffEvalFunction(color_set,vertex,cacndidate_color);
                color_set.MoveVertexColor( vertex,cacndidate_color );
                break;
            }
        } 
    }
    return color_set;
}
//どんな色分けになるかのカラーセット(評価関数はおかしいまま),どの頂点を移動すれば良いかを返す
//テストクリア(テストケース... _CalcMoveDistanceTest) : バグ残っててもおかしくないのでcerrは残す
pair<ColorSet,set<int>> TSHwithPR::PathRelinking::CalcMoveDistance(ColorSet initial_set,ColorSet guiding_set) {
    //論文通りの方が確実なのでやる。
    ColorSet ans_set;
    ans_set = initial_set;
    set<int> confirm_vertexes;
    set<int> move_vertexes;
    guiding_set.GreaterSort();
    vector<ColorClass> guiding_color_classes = guiding_set.GetColorSet();
     
    for ( int target_color = 0; target_color < guiding_set.GetColorSet().size(); target_color++ ) {
        if(guiding_color_classes[target_color].vertexes.empty()) break; //sortしてあるのでbreakでok
        vector<pair<set<int>,int>> color_pertition(guiding_set.GetColorSet().size());
        for(int i = 0; i < ans_set.GetColorSet().size(); i++) {
            color_pertition[i].second = i;
            color_pertition[i].first = IntersectionSet2Set(guiding_color_classes[target_color].vertexes,ans_set.GetColorSet()[i].vertexes);
        }
        //揃ってる部分が多い順にしたい
        sort( color_pertition.begin(),color_pertition.end(),pair_set_int_comp_greater );
        bool move = false; //下のfor抜けた後のフラグとして使う
        //9行目のwhileの意味あとで考える
        for (int pertition = 0; pertition < color_pertition.size(); pertition++) {
            if(color_pertition[pertition].first.empty() || move) break;
            
            int ans_set_pertition = color_pertition[pertition].second;
            set<int> tmp_minus_pertition = RemoveSet2Set(ans_set.GetColorSet()[pertition].vertexes,color_pertition[ans_set_pertition].first);
            if( (!set_int_equal(color_pertition[pertition].first ,ans_set.GetColorSet()[ans_set_pertition].vertexes)) && 
            (!IntersectionSet2Set(tmp_minus_pertition,confirm_vertexes).empty())) continue;
            move = true; 
            for(int vertex : guiding_color_classes[target_color].vertexes) {
                if(color_pertition[pertition].first.find(vertex) != color_pertition[pertition].first.end() ) continue;
                ans_set.MoveVertexColor(vertex,ans_set_pertition);
            }
            set<int> insert_set = RemoveSet2Set(guiding_color_classes[target_color].vertexes,color_pertition[pertition].first);
            move_vertexes.insert(insert_set.begin(),insert_set.end());
            confirm_vertexes.insert(guiding_color_classes[pertition].vertexes.begin(),guiding_color_classes[pertition].vertexes.end());
        }
        if(!move) {
            bool roop=true;
            int new_color = 0;
            while(roop) {
                if(ans_set.GetColorSet()[new_color].vertexes.empty()) roop = false;
                else new_color++;
            }
            for( int vertex : guiding_set.GetColorSet()[target_color].vertexes) {
                ans_set.MoveVertexColor(vertex,new_color);
            }
            move_vertexes.insert(guiding_color_classes[target_color].vertexes.begin(),guiding_color_classes[target_color].vertexes.end());
            confirm_vertexes.insert(guiding_color_classes[target_color].vertexes.begin(),guiding_color_classes[target_color].vertexes.end());
        }
    }
    return { ans_set, move_vertexes };
}

ColorSet TSHwithPR::PathRelinking::CalcPathRelinking(ColorSet initial_S,ColorSet goal_S,set<int> move_vertexes) {
    ColorSet path_S = initial_S;
    ColorSet ans_color_set = initial_S;
    while(!move_vertexes.empty()) {
        long long min_eval = INF;
        int move_vertex = -1;
        // 最終的なカラーリングにちかづけたい。もっとも評価が良いやつから順に変えていく
        for (int vertex : move_vertexes) {
            long long tmp_score = DiffEvalFunction(path_S,vertex,goal_S.GetSearchColor()[vertex]);
            if(tmp_score < min_eval) {
                min_eval = tmp_score;
                move_vertex = vertex;
            }
        }
        path_S.MoveVertexColor(move_vertex,goal_S.GetSearchColor()[move_vertex]);
        path_S.score = min_eval;
        move_vertexes.erase(move_vertex);
        if(path_S.score < ans_color_set.score) { 
            ans_color_set = path_S;
        }
    }
    return ans_color_set;
}
//テストクリア(テストケース... PathRelinkingTest)
ColorSet TSHwithPR::Perturbation::SetRandomColor(ColorSet target_color_set,int max_perturbation = sqrt(graph.size())) { 
    tabu_list = {};
    //変更する頂点を定める。上から max_perturbation順にとっていけば重複なしのランダム抽出に鳴る
    vector<int> change_vertex;
    for (int i = 0; i < graph.size(); i++)  change_vertex.push_back(i);
    change_vertex = shuffle( change_vertex , mt);
    for ( int i = 0; i < max_perturbation; i++) {
        int vertex = change_vertex[i];
        int tmp_color;
        while(true) {
            if( target_color_set.num_color < graph.size() ) {
                tmp_color = mt() % (target_color_set.num_color+1);
            } else {
                tmp_color = mt() % target_color_set.num_color;
            }
            if(tmp_color != target_color_set.GetSearchColor()[vertex]) break;
        }
        target_color_set.score = DiffEvalFunction(target_color_set,vertex,tmp_color);
        target_color_set.MoveVertexColor(vertex,tmp_color);
        VertexMove tabu_insert = {vertex,tmp_color};
        tabu_list.insert(tabu_insert);
    }
    return target_color_set;
}
//テストクリア(テストケース... _EliteSetUpdateTest)
void TSHwithPR::EliteSetUpdate::PriorHighScore(ColorSet target_color_set) {
    //elite setにかぶりがあるなら更新しない
    if( EliteSetHaveElement(elite_set,target_color_set) ) return;
    target_color_set.score = EvalFunction( target_color_set );
    if(elite_set.size() < elite_max_size) { 
        elite_set.push_back(target_color_set);
        return;
    }
    long long warst_score = elite_set[0].score;
    int warst_pnt = 0;
    for (int i = 1; i < elite_set.size();i++ ) {
        if(warst_score < elite_set[i].score ) { 
            warst_score = elite_set[i].score;
            warst_pnt = i;
        }
    }
    if( warst_score > target_color_set.score ) elite_set[warst_pnt] = target_color_set;
}

bool TSHwithPR::EliteSetUpdate::EliteSetHaveElement(vector<ColorSet> target_elite,ColorSet target_color_set) {
    for(ColorSet color_set : target_elite) {
        if(color_set == target_color_set) return true;
    }
    return false;
}

void TSHwithPR::ResetData() {
    elite_set.clear();
    graph.clear();
    current_color = reset_color;
    init_color = reset_color;
}

ColorSet TSHwithPR::Result() {
    ColorSet ans_color_set;
    ans_color_set.score = INF;
    for (ColorSet color_set : elite_set) {
        if(ans_color_set.score > color_set.score) ans_color_set = color_set;
    }
    return ans_color_set;
}
//最終実行
void TSHwithPR::Run() {
    clock_t start = clock();
    InputGraph();
    InputColorSet();
    Greedy::ReassignLargestCardinality(0.9);
    current_color.score = EvalFunction(current_color);
    _ShowColorSet(current_color);
    do {
        bool conflict_check = true;
        while(conflict_check){ 
            auto [tmp, check , tmp_color ] = Greedy::CriticalOneMoveNeighborhood( current_color );
            current_color = tmp_color;
            conflict_check = check;
            //_ShowColorSet(current_color);
        }
        current_color = LocalSearch::Reassign2SmallerOne( current_color );
        EliteSetUpdate::PriorHighScore( current_color );
        ColorSet Sguiding = elite_set[mt() % elite_set.size()];
        if(elite_set.size() < 2) continue;
        auto [ Sgoal, move_vertexes ] = PathRelinking::CalcMoveDistance ( current_color,Sguiding );
        if(move_vertexes.size() < 2) continue;
        current_color = PathRelinking::CalcPathRelinking( current_color,  Sgoal ,move_vertexes );
        //PathRelinking::BeamSearch::NodeSearch(current_color, Sgoal , move_vertexes );
        //current_color = PathRelinking::BeamSearch::Output();
        current_color = LocalSearch::Reassign2SmallerOne( current_color );
        EliteSetUpdate::PriorHighScore( current_color );
        current_color = Perturbation::SetRandomColor( current_color,sqrt( graph.size() ) );
        cout << "aa" << endl;
    }while( double( clock() - start ) / CLOCKS_PER_SEC < 15.0 );
    cout << "結果" << endl;
    // floatによって桁起してるから最後にしっかり調整する -> long long にした
    //current_color.score = EvalFunction(current_color);
    _ShowColorSet( Result() );
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
//テストクリア(テストケース... _Reassign2SmallerOneTest)
    cout << endl;
}
void TSHwithPR::_RemoveSet2SetTest() {
    set<int> a = {1,2,3,4,5,6,7,8,9};
    set<int> b = {1,3,5,7,9};
    set<int> c = {1,2,3,4,5,6,7,8,9};
    set<int> ans = RemoveSet2Set(a,c);
    for(int vertex : ans){
        cout << vertex << " ";
    }
    cout << endl;
}
/*
void TSHwithPR::_MoveVertexColorTest() {
    ColorSet cs;
    cs.score = 100;
    cs.S.resize(10);
    for(int i=0;i<20;i++) {
        cs = MoveVertexColor( cs, i, i%5 );
//テストクリア(テストケース... _Reassign2SmallerOneTest)
    }
    _ShowColorSet(cs);
    for(int i=0;i<5;i++) { 
        cs = MoveVertexColor( cs , i , 2 );
    }
    _ShowColorSet(cs);
}
*/
void TSHwithPR::_ReassignTest() {
    InputGraph();
    InputColorSet();
    Greedy::ReassignLargestCardinality(0.9);
    current_color.score = EvalFunction(current_color);
    _ShowColorSet(current_color);
    //exit(0);
    bool ok = true;
    while(ok) { //Criticalを回す条件式は後で考える必要あり
        auto[ a,tmp, tmp_color ] = Greedy::CriticalOneMoveNeighborhood(current_color);
        ok = tmp;
        //_ShowColorSet(current_color);
    }
    _ShowColorSet(current_color);
}
//たまにスコアがマイナスになるバグがある(直した)
void TSHwithPR::_Reassign2SmallerOneTest() {
    _ReassignTest();
    current_color = LocalSearch::Reassign2SmallerOne(current_color);
    current_color.score = EvalFunction(current_color);
    _ShowColorSet(current_color);
}
/*
void TSHwithPR::_PathRelinkingTest() { //元はCalcDistanceTest
    ColorSet Sinitial,Sguiding;
    InputGraph();
    InputColorSet();
    Sinitial = init_color;
    Sguiding = init_color;
    // グラフ手計算に書いてあるカラーリングをメモ
    Sinitial.S[0].vertexes.insert(3);
    Sinitial.S[0].vertexes.insert(5);
    Sinitial.S[0].vertexes.insert(1);
    Sinitial.S[0].vertexes.insert(2);
    Sinitial.S[0].vertexes.insert(0);
    Sinitial.S[0].vertexes.insert(4);
    Sinitial.search_color[3] = 0;
    Sinitial.search_color[5] = 0;
    Sinitial.search_color[1] = 0;
    Sinitial.search_color[2] = 0;
    Sinitial.search_color[0] = 0;
    Sinitial.search_color[4] = 0;
    Sinitial.num_color = 1;

    Sguiding.S[1].vertexes.insert(0);
    Sguiding.S[1].vertexes.insert(3);
    Sguiding.S[1].vertexes.insert(4);
    Sguiding.S[0].vertexes.insert(1);
    Sguiding.S[0].vertexes.insert(2);
    Sguiding.S[2].vertexes.insert(5);
    Sguiding.search_color[0] = 1;
    Sguiding.search_color[3] = 1;
    Sguiding.search_color[4] = 1;
    Sguiding.search_color[1] = 0;
    Sguiding.search_color[2] = 0;
    Sguiding.search_color[5] = 2;
    Sinitial.num_color = 3;

    Sguiding.score = EvalFunction(Sguiding);
    Sinitial.score = EvalFunction(Sinitial);
    auto[ ans_color_set , move_vertexes ] = PathRelinkng::CalcMoveDistance( Sinitial, Sguiding );
    _ShowColorSet(ans_color_set);
    ColorSet after_pathrelinking = PathRelinkng::PathRelinking(Sinitial,Sguiding,ans_color_set,move_vertexes);
    _ShowColorSet(after_pathrelinking);
    after_pathrelinking = Perturbation::SetRandomColor(after_pathrelinking);
    _ShowColorSet(after_pathrelinking);
}
*/
void TSHwithPR::_EliteSetUpdateTest() {
    ColorSet a[20];
    for (int i = 0; i < 20; i++) {
        a[i].score = (i*13) % 7;
        EliteSetUpdate::PriorHighScore(a[i]);
        for( ColorSet c : elite_set) cout << c.score << " ";
        cout << endl;
    }

}