#include "../include/TSHwithPR.hpp"
using namespace std;
vector<vector<int>> TSHwithPR::PathRelinking::DAG::dag_graph;
vector<int> TSHwithPR::PathRelinking::DAG::dag_weight;
vector<vector<int>> TSHwithPR::PathRelinking::DAG::reverse_dag_graph; //逆向きの有向グラフ.逆向きに参照したい時に使う
vector<vector<int>> TSHwithPR::PathRelinking::DAG::dag2original_vertexes;//dagの頂点から元の有向グラフ時の頂点番号を引き出すやつ
vector<set<int>> TSHwithPR::PathRelinking::DAG::transitive_closure; //推移閉包。アルゴリズムにも使えそうだし、あると便利

struct TSHwithPR::PathRelinking::DAG::StronglyConnectedComponents {
  private:
    const vector<vector<int>> &original_graph; //頂点番号,行き先
    vector<vector<int>> normal_graph, reverse_graph;   // normal graph,reverse graph
    vector<int> compornent, order,used; // compo:どの頂点集合に属しているか,order:帰りがけ順の割り振り
    
  public:
    StronglyConnectedComponents(vector<vector<int>> &original_graph) 
    : original_graph(original_graph), normal_graph(original_graph.size()), reverse_graph(original_graph.size()), 
      compornent(original_graph.size(), -1), used(original_graph.size(), 0) 
    {
        for(int i = 0; i < original_graph.size(); i++) {
            for(int t : original_graph[i]) {
                normal_graph[i].push_back(t);
                reverse_graph[t].push_back(i);
            }
        }
    }

    int operator[](int k) const { return compornent[k]; }
    // 1回目dfs.帰りがけ順を記録

    vector<vector<int>> build() {
        for(int i = 0; i < normal_graph.size(); i++) dfs(i);
        reverse(order.begin(), order.end());
        int group = 0;
        for(int i : order) {
            if(compornent[i] == -1) rdfs(i, group++);
        }
        vector<vector<int>> result(group);
        //すべての辺をみていって、同じgroupに属していなかった場合に辺をつなぐ
        for(int i = 0; i < original_graph.size(); i++) {
            for(int to : original_graph[i]) {
                int s = compornent[i], t = compornent[to];
                if(s != t) result[s].emplace_back(t);
            }
        }
        return result;
    }
  private:
    void dfs(int now) {
        if(used[now]) return;
        used[now] = true;
        for(int to : normal_graph[now]) dfs(to);
        order.emplace_back(now); // 帰りがけ順に頂点番号を入れていく
    }

    // 2回目dfs.帰りがけ順を逆側からみていき、逆順でも辿れる場所があるなら合流
    void rdfs(int now, int group) {
        if(compornent[now] != -1) return;
        compornent[now] = group; //強連結成分をひとつにまとめる
        for(int to : reverse_graph[now]) rdfs(to, group);
    }
};


//テストケース _test2
vector<vector<int>> TSHwithPR::PathRelinking::DAG::MakeUndirectedGraph(const vector<int> &move_vertexes) {
    vector<vector<int>> answer_graph(move_vertexes.size());
    for( int v1 = 0; v1 < move_vertexes.size(); v1++ ) {
        for(int v2 = v1+1; v2 < move_vertexes.size(); v2++ ) {
            //graphの入力は昇順なので、2分探索できる
            int mv1 = move_vertexes[v1];
            int mv2 = move_vertexes[v2];
            if(lower_bound(graph[mv1].begin(),graph[mv1].end(),mv2) - upper_bound(graph[mv1].begin(),graph[mv1].end(),mv2) != 0) {
                answer_graph[v1].emplace_back(v2);
                answer_graph[v2].emplace_back(v1);
            }
        }
    }
    return answer_graph;
};
//テストケース _test2
std::vector<std::vector<int>> TSHwithPR::PathRelinking::DAG::MakeDirectedGraph(const vector<int> &move_vertexes,const ColorSet &init_set,const ColorSet &guiding_set) {
    vector<vector<int>> answer_graph(move_vertexes.size());
    vector<vector<int>> undirected_graph = MakeUndirectedGraph(move_vertexes);
    map<int,int> init_search_color = init_set.GetSearchColor();
    map<int,int> guiding_search_color = guiding_set.GetSearchColor();
    for( int v1 = 0; v1 < move_vertexes.size(); v1++ ) {
        int mv1 = move_vertexes[v1];
        for(int v2 : undirected_graph[v1] ) { 
            int mv2 = move_vertexes[v2];
            int mv2color = init_search_color[mv2];
            int change_mv1color = guiding_search_color[mv1];
            //色1の色変後と、色2の変更前が同じ色なら1 -> 2
            if( change_mv1color == mv2color)  answer_graph[v1].emplace_back(v2);
        }
    }
    return answer_graph;
};
//頂点集合を重みわけする
vector<int> TSHwithPR::PathRelinking::DAG::MakeDagWeight(const StronglyConnectedComponents &SCC, const vector<int> &move_vertexes,const ColorSet &init_set,const ColorSet &guiding_set) {
    vector<int> answer_weight(dag_graph.size(),0);
    map<int,int> guiding_color = guiding_set.GetSearchColor();
    map<int,int> init_color = init_set.GetSearchColor();
    vector<ColorClass> guiding_color_classes = guiding_set.GetColorSet();
    vector<ColorClass> init_color_classes = init_set.GetColorSet();
    for( int i = 0; i < move_vertexes.size(); i++ ) {
        int mv = move_vertexes[i];
        int change_score = guiding_color_classes[guiding_color[mv]].weight - init_color_classes[init_color[mv]].weight;
        answer_weight[SCC[i]] += change_score;
    }
    return answer_weight;
};
//未テスト
vector<vector<int>> TSHwithPR::PathRelinking::DAG::MakeDag2OriginalVertexes(const StronglyConnectedComponents &SCC,const vector<int> &move_vertexes) {
    vector<vector<int>> answer(dag_graph.size());
    for( int v = 0; v < move_vertexes.size(); v++ ) {
        answer[SCC[v]].emplace_back( v );
    }
    return answer;
}
//テストケース_test2
void TSHwithPR::PathRelinking::DAG::MakeReverseDagGraph() {
    reverse_dag_graph.clear();
    reverse_dag_graph.resize(dag_graph.size());
    for( int i = 0; i < dag_graph.size(); i++ ) {
        for( int j : dag_graph[i] ) {
            reverse_dag_graph[j].emplace_back(i);
        }
    }
}
//BFSテスト済み
void TSHwithPR::PathRelinking::DAG::MakeTransitiveClosure() { //計算量に妥協有り
    int n = dag_graph.size();
    //隣接行列
    transitive_closure.clear();
    transitive_closure.resize(n);
    for (int tv= 0; tv < n; tv++ ) {
        queue<int> child;
        child.push( tv );
        while( !child.empty() ) {
            int v = child.front(); child.pop();
            for( int m : dag_graph[v] ) {
                transitive_closure[tv].insert(m);
                child.push(m);
            }
        }
    }
}
/***
    //warshall_floyd.単にDFS,BFSで事足りる.隣接行列で使いたい時に使う
void TSHwithPR::PathRelinking::DAG::MakeTransitiveClosure() { //計算量に妥協有り
    transitive_closure.resize(n, vector<bool>( n , false ));
    for (int i = 0; i < dag_graph.size(); i++) {
        for ( int j : dag_graph[i] ) {
            transitive_closure[i][j] = true; //辺が通っているならtrue
        }
    }
    for( int k = 0; k < n; k++ ) {
        for ( int i = 0; i < n; i++ ) {
            for ( int j = 0; j < n; j++ ) {
                if( transitive_closure[i][k] == false || transitive_closure[k][j] == false ) continue;
                transitive_closure[i][j] = true;
            }
        }
    }
}
***/
//ランダムテスト完璧合格
void TSHwithPR::PathRelinking::DAG::Build( const vector<int> &move_vertexes, const ColorSet &init_set, const ColorSet &guiding_set ) {
    vector<vector<int>> directed_graph =  MakeDirectedGraph( move_vertexes , init_set , guiding_set );
    StronglyConnectedComponents SCC( directed_graph );
    dag_graph = SCC.build();
    MakeReverseDagGraph();
    MakeTransitiveClosure();
    dag_weight = MakeDagWeight( SCC, move_vertexes , init_set , guiding_set );
    dag2original_vertexes = MakeDag2OriginalVertexes( SCC, move_vertexes );
}
//未テスト
//推移閉包を作って、そこと同じになった頂点すべてを確定させる必要あり
//cntを踏む回数が多いのはおかしい。そこが悪さしてそう
vector<bool> TSHwithPR::PathRelinking::DAG::DagGreedy() {
    vector<bool> answer( dag_graph.size(),false ); //非確定のものはfalseでok
    queue<int> leaf; //出し入れ楽そうだから。queueじゃなくてもいける
    vector<int> set_vertexes( dag_graph.size() ); //1つにまとめた頂点を記録しておく(親に子を記録)
    vector<vector<int>> tmp_dag = dag_graph;
    vector<int> tmp_dag_weight = dag_weight;
    //もし元のdagグラフの頂点にそこから発の辺が無い場合は葉
    for(int v=0 ; v < tmp_dag.size(); v++) {
        if( !tmp_dag[v].empty() ) continue;
        leaf.push(v);
    }
    //疑似コード通りに書く
    while(!leaf.empty()) {
        int v = leaf.front(); leaf.pop();
        if ( tmp_dag_weight[v] <= 0) {
            answer[v] = true; //確定
            for( int l  : reverse_dag_graph[v] ) { //親からの枝を消す
                tmp_dag[l].erase( remove( tmp_dag[l].begin(), tmp_dag[l].end(), v ), tmp_dag[l].end() );
                if( tmp_dag[l].empty() ) leaf.push(l); ///消した結果emptyなら葉
            }
        } else { //確定しない時の処理
            int num_parent = reverse_dag_graph[v].size(); //親の数
            for( int l : reverse_dag_graph[v] ) {
                //小数点切り捨て。floatにしなかったことの弊害
                tmp_dag_weight[l] += tmp_dag_weight[v]  / num_parent;
                tmp_dag[l].erase( remove( tmp_dag[l].begin(), tmp_dag[l].end(), v ), tmp_dag[l].end() );
                if( tmp_dag[l].empty() ) leaf.push(l); ///消した結果emptyなら葉
            } 
        }
    }
    //まとめて色変する頂点を推移閉包の隣接行列でチェック
    for( int i = 0; i < answer.size(); i++ ) {
        if( !answer[i] ) continue;
        for( int j : transitive_closure[i] ) {
            answer[j] = true;
        }
    }
    //帰り値は元のdag頂点番号で返す
    return answer;
}

//未テスト
vector<int> TSHwithPR::PathRelinking::DAG::CalcChangesVertexes( const vector<int> &move_vertexes ) {
    vector<int> answer;
    vector<bool> dag_changes_vertexes = DagGreedy();
    for( int v = 0; v < dag_changes_vertexes.size(); v++ ) {
        if ( !dag_changes_vertexes[v] )  continue;
        for( int l : dag2original_vertexes[v] ) { //dagの頂点 -> 有向グラフに戻した時の頂点集合
            answer.emplace_back( move_vertexes[l] ); //有向グラフの頂点番号->元のグラフの頂点番号
        }
    } 
    return answer;
}

void TSHwithPR::PathRelinking::DAG::_test2() {
    ColorSet Sinitial,Sguiding;
    InputGraph();
    InputColorSet();
    Sinitial = init_color;
    Sguiding = init_color;
    // グラフ手計算に書いてあるカラーリングをメモ
    /*
    Sinitial.S[2].vertexes.insert(3);
    Sinitial.S[2].vertexes.insert(5);
    Sinitial.S[0].vertexes.insert(1);
    Sinitial.S[1].vertexes.insert(2);
    Sinitial.S[0].vertexes.insert(0);
    Sinitial.S[3].vertexes.insert(4);
    Sinitial.search_color[3] = 2;
    Sinitial.search_color[5] = 2;
    Sinitial.search_color[1] = 0;
    Sinitial.search_color[2] = 1;
    Sinitial.search_color[0] = 0;
    Sinitial.search_color[4] = 3;
    Sinitial.num_color = 4;

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
    Sguiding.num_color = 3;
*/
    Sguiding.score = EvalFunction(Sguiding);
    Sinitial.score = EvalFunction(Sinitial);
    Build({0,2,3,4},Sinitial,Sguiding);
    cout << "dag graph" << endl;
    for( vector<int> v : dag_graph) {
        for( int e : v) {
            cout << e << " ";
        }
        cout << endl;
    }
    cout << "reverse_dag graph" << endl;
    for( vector<int> v : reverse_dag_graph) {
        for( int e : v) {
            cout << e << " ";
        }
        cout << endl;
    }
    cout << "dag weight" << endl;
    for(int w : dag_weight) {
        cout << w << " ";
    }
    cout << endl;
}

//SCCのテスト
void TSHwithPR::PathRelinking::DAG::_test1() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);
    int n,m; cin >> n >> m;
    vector<vector<int>> graph(n);
    for (int i=0; i<m; i++) {
        int a,b; 
        cin >> a >> b;
        graph[a].push_back(b);
    }
    StronglyConnectedComponents SCC( graph );
    vector<vector<int>> result = SCC.build();
    vector<vector<int>> out(510000); 
    int o = 0;
    for(int i=0;i<n;i++) {
        out[SCC[i]].push_back(i);
        if(o < SCC[i]) o = SCC[i];
    }
    cout << o+1 << endl;
    for(vector<int> v : out) {
        if(v.empty()) break;
        cout << v.size() << " ";
        for(int i : v) cout << i << " ";
        cout << endl; 
    }
}