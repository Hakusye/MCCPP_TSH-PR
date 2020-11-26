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
    InputGraph();
    InputColorSet();
    clock_t start = clock();
    Train("log/test.score");
    cout << "total time:" << clock() - start << endl;
}

void AnalizerForTSH::Input4Train(string graph_name ,string color_name,string log_name = "log/test.score") {
    InputGraph( graph_name );
    InputColorSet( color_name );
    Train( log_name );
}

// 色々込み込みで実行するやつ
void AnalizerForTSH::Train(string log_name ="log/test.score") {
    clock_t iter_time = clock();
    int num = 0;
    Greedy::ReassignLargestCardinality(0.9);
    current_color.score = EvalFunction(current_color);
    _ShowColorSet(current_color);
    do {
        num++;
        bool conflict_check = true;
        int tep = 0; //テストするための変数
        // 8/9 ... MoveVertex後の話。proper_colorにする過程では停止していなそう
        clock_t greedy_time = clock();
        while(conflict_check) { 
            auto [ conf, check , tmp_color ] = Greedy::CriticalOneMoveNeighborhood( current_color );
            current_color = tmp_color;
            conflict_check = check;
            if(conf.size() == 1) tep++;
            if(tep == 10) {
                // バグったときのグラフ出力
                for( int vertex : conf) {
                    cout << "衝突頂点" << endl;
                    cout << vertex << " " << tmp_color.GetSearchColor()[vertex] << endl;
                    cout << "隣接頂点" << endl;
                    for(int vadja :graph[vertex] ) {
                        cout << vadja << " " << tmp_color.GetSearchColor()[vadja] << endl;
                    }
                    cout << "最終カラリング" << endl;
                    _ShowColorSet(tmp_color);
                }
                exit(1);
            }
            //_ShowColorSet(current_color);
        }
        clock_t conflict_time = clock();
        current_color = LocalSearch::Reassign2SmallerOne( current_color );
        EliteSetUpdate::PriorHighScore( current_color );
        ColorSet Sguiding = elite_set[mt() % elite_set.size()];
        if(elite_set.size() < 2) {
            current_color = Perturbation::SetRandomColor( current_color,sqrt( graph.size() ) );
            continue;
        }
        clock_t local_search_time = clock();
        auto [ Sgoal, move_vertexes ] = PathRelinking::CalcMoveDistance ( current_color,Sguiding );
        clock_t  move_distance_time = clock();
        if(move_vertexes.size() >= 3) {
            cout << "iterator:"<< num << endl;
            //cout  << move_vertexes.size() << endl;

            PathRelinking::BeamSearch::NodeSearch( current_color, Sgoal ,move_vertexes ); 
            current_color = PathRelinking::CalcPathRelinking( current_color, Sgoal ,move_vertexes );
            //current_color = PathRelinking::BeamSearch::Output( ); 
            cout << "beam_search - greedy = "<< PathRelinking::BeamSearch::Output().score - current_color.score   << endl;
            //cout << "after output" << endl;
            current_color = LocalSearch::Reassign2SmallerOne( current_color );
            //cout << "smallerone" << endl;
            EliteSetUpdate::PriorHighScore( current_color );
        }
        clock_t path_relink_time = clock();
        current_color = Perturbation::SetRandomColor( current_color,sqrt( graph.size() ) );
        //current_color = Perturbation::SetRandomColor( current_color,30 );
        if( num % 10 == 9 ) {
            SaveLearningCurve(num/5,log_name);
            //cout << "greedy:" << greedy_time - iter_time << endl;
            //cout << "improve_comflict:" <<  conflict_time - greedy_time << endl;
            //cout << "local_search:" << local_search_time - conflict_time << endl;
            //cout << "move distance:" << move_distance_time - local_search_time << endl;
            //cout << "path relinking:" << path_relink_time - move_distance_time << endl;
        }
        iter_time = clock();
    }while( num != 20 );
    cout << "結果" << endl;
    // floatによって桁起してるから最後にしっかり調整する -> long long にした
    //current_color.score = EvalFunction(current_color);
    _ShowColorSet( Result() );
}
