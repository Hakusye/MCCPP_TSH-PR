#include "../include/AnalizerForTSH.hpp"
#include "../include/util.hpp"
using namespace std;
//保存したファイルをグラフィックで出力:
void AnalizerForTSH::ShowLearningCurve() {

}
// EliteSet平均距離差,ベストスコア,EliteSet内のスコア平均を記録する
void AnalizerForTSH::SaveLearningCurve(int itr,string filename) {
    ofstream ofs( filename, ios::app );
    if(!ofs) {
        cout << filename << "ファイルに書き込めません" << endl;
        cin.get();
        exit(1);
    }
    auto [ distance_average, best_score, average_score ] = CalcDistanceAndScoreInEliteSet();
    ofs << itr << " " << distance_average << " " << best_score << " " << average_score << "\n";
}
// 距離差を計算し保存する itr,average scoore
//関連させるのが面倒なので一回一回全組み合わせを列挙している(かなり遅いかも)
tuple<double,long long, long long> AnalizerForTSH::CalcDistanceAndScoreInEliteSet() {
    double distance_average=0;
    long long best_score=INF;
    long long sum_score = 0;
    for( int i = 0; i < elite_set.size(); i++ ) {
        cout << elite_set[i].score << " ";
        for( int j = i+1; j < elite_set.size(); j++ ) {
            auto [ tmp,score ] = PathRelinking::CalcMoveDistance(elite_set[i],elite_set[j]);
            distance_average += score.size();
        }
        best_score = min( best_score,elite_set[i].score );
        sum_score += elite_set[i].score;
    }
    cout << endl;
    distance_average = (distance_average * 2.0 ) / double(elite_set.size() * (elite_set.size()+1));
    
    return { distance_average, best_score, sum_score / elite_set.size() };
}

void AnalizerForTSH::Analize() {
    string graph_path = "dataset/graph/";
    string weight_path = "dataset/weight/";
    string log_ext = ".score";
    vector<pair<string,string>> files = {
        {"DSJC125.1.col","125_max1000_random.dat"},
        //{"DSJC125.5.col","125_max1000_random.dat"},
        {"DSJC125.9.col","125_max1000_random.dat"},
        {"DSJC250.1.col","125_max1000_random.dat"},
        {"DSJC250.5.col","125_max1000_random.dat"},
        //{"DSJC500.1.col","125_max1000_random.dat"},
        {"flat300_20_0.col","125_max1000_random.dat"},
        {"games120.col","120_max1000_random.dat"}
    };
    for(pair<string,string> file : files) {
        Input4Train(graph_path+file.first, weight_path+file.second, "log/"+file.first+log_ext);
        ResetData();
    }
}

void AnalizerForTSH::Input4Train() {
    clock_t start = clock();
    int maxi = -1;
    int mini = 1e9+7;
    float sum=0;
    float average;
    InputGraph( );
    InputColorSet( );
    for( int i = 0 ;i < 10; i++) {
        cout << "試行回数:" <<  i << endl;
        int tmp = Train( "log/test.score" );
        maxi = max( maxi, tmp );
        mini = min( mini, tmp );
        sum += tmp;
        elite_set.clear();
        //current_color.InitColorSet(init_color);
        current_color = init_color;
    }
    average = sum / 10.0;
    cout << "min:" << mini << endl;
    cout << "max:" << maxi << endl;
    cout << "averavge:" << average << endl;
    cout << "total time:" << (clock() - start) / CLOCKS_PER_SEC << endl;
}

void AnalizerForTSH::Input4Train(string graph_name ,string color_name,string log_name = "log/test.score") {
    int maxi = -1;
    int mini = 1e9+7;
    float sum=0;
    float average;
    InputGraph( graph_name );
    InputColorSet( color_name );
    for( int i = 0 ;i < 1; i++) {
        cout << "試行回数:" <<  i << endl;
        int tmp = Train( log_name );
        maxi = max( maxi, tmp );
        mini = min( mini, tmp );
        sum += tmp;
    }
    average = sum / 10.0;
    cout << "min:" << mini << endl;
    cout << "max:" << maxi << endl;
    cout << "averavge:" << average << endl;
}

// 色々込み込みで実行するやつ
//普段はvoid。実験自動化のため
int AnalizerForTSH::Train(string log_name ="log/test.score") {
    clock_t iter_time = clock();
    int num = 0;
    Greedy::ReassignLargestCardinality(0.95);
    current_color.score = EvalFunction(current_color);
    //_ShowColorSet(current_color);
    do {
        num++;
        cout << "iterator:"<< num << endl;
        // 8/9 ... MoveVertex後の話。proper_colorにする過程では停止していなそう
        clock_t greedy_time = clock();
        while(true) {  //checkがfalseのとき(衝突がない)抜ける
            auto [ conf, check , increase_color, change_vertex,change_color ] = Greedy::CriticalOneMoveNeighborhood( current_color );
            //current_color = tmp_color;
            if(!check) break;
            if(increase_color) current_color.num_color++;
            current_color.score = DiffEvalFunction( current_color, change_vertex,change_color );
            current_color.MoveVertexColor( change_vertex, change_color );
        }
        current_color.score = EvalFunction(current_color);
        clock_t conflict_time = clock();
        current_color = LocalSearch::Reassign2SmallerOne( current_color );
        //cout << "local search:" << current_color.score << endl;
        EliteSetUpdate::PriorHighScore( current_color );
        ColorSet Sguiding = elite_set[mt() % elite_set.size()];
        if(elite_set.size() < 2) {
            current_color = Perturbation::SetRandomColor( current_color,sqrt( graph.size() ) );
            continue;
        }
        clock_t local_search_time = clock();
        Sguiding.GreaterSort();
        auto [ Sgoal, move_vertexes ] = PathRelinking::CalcMoveDistance ( current_color,Sguiding );
        if( move_vertexes.size() >= 3 ) {
            // path_relinking周り
            //current_color = PathRelinking::CalcPathRelinking( current_color, Sgoal ,move_vertexes );
            //PathRelinking::BeamSearch::NodeSearch( current_color, Sgoal ,move_vertexes ); 
            //current_color = PathRelinking::BeamSearch::Output( ); 
            //cout << "beam_search - greedy = "<< PathRelinking::BeamSearch::Output().score - current_color.score   << endl;
            //DAGのやつ
             
            vector<int> move_vertexes_vec( move_vertexes.begin(),move_vertexes.end() );
            TSHwithPR::PathRelinking::DAG::Build(move_vertexes_vec ,current_color ,Sgoal);
            vector<int> change_vertexes = TSHwithPR::PathRelinking::DAG::CalcChangesVertexes( move_vertexes_vec );
            for( int v : change_vertexes ) {
               current_color.score = DiffEvalFunction( current_color, v, Sgoal.GetSearchColor()[v] );
                current_color.MoveVertexColor( v , Sgoal.GetSearchColor()[v] );
            }
            
            //事後処理
            current_color = LocalSearch::Reassign2SmallerOne( current_color );
            EliteSetUpdate::PriorHighScore( current_color );
        }
        clock_t path_relink_time = clock();
        current_color = Perturbation::SetRandomColor( current_color,sqrt( graph.size() ) );
        //current_color = Perturbation::SetRandomColor( current_color,30 );
        if( num % 10 == 9 ) {
            SaveLearningCurve(num/5,log_name);
        }
        iter_time = clock();
    }while( num != 50 );
    cout << "結果" << endl;
    // floatによって桁起してるから最後にしっかり調整する -> long long にした
    //current_color.score = EvalFunction(current_color);
    _ShowColorSet( Result() );
    long long ans = 1e9+7;
    for(int i=0; i < elite_set.size(); i++) {
        ans = min( ans,elite_set[i].score );
    }
    return ans;
}
