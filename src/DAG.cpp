#include "../include/TSHwithPR.hpp"
using namespace std;
vector<vector<int>> TSHwithPR::PathRelinking::DAG::dag_graph;
vector<int> TSHwithPR::PathRelinking::DAG::dag_weight;

struct TSHwithPR::PathRelinking::DAG::StronglyConnectedComponents {
  private:
    const vector<vector<int>> &original_graph; //頂点番号,行き先
    vector<vector<int>> normal_graph, reverse_graph;   // normal graph,reverse graph
    vector<int> compornent, order,used; // compo:どの頂点集合に属しているか,order:>帰りがけ順の割り振り
    
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
            //色1をの色変後と、色2の変更前が同じ色なら1 -> 2
            if( change_mv1color == mv2color)  answer_graph[v1].emplace_back(v2);
        }
    }
    return answer_graph;
};
//頂点集合を重みわけする
std::vector<int> TSHwithPR::PathRelinking::DAG::MakeDagWeight(const StronglyConnectedComponents &SCC, const vector<int> &move_vertexes,const ColorSet &init_set,const ColorSet &guiding_set) {
    vector<int> answer_weight(dag_graph.size(),0);
    map<int,int> guiding_color = guiding_set.GetSearchColor();
    map<int,int> init_color = init_set.GetSearchColor();
    for( int i = 0; i < move_vertexes.size(); i++ ) {
        int mv = move_vertexes[i];
        int change_score = guiding_set.GetColorSet()[guiding_color[mv]].weight - init_set.GetColorSet()[init_color[mv]].weight;
        answer_weight[SCC[i]] += change_score;
    }
    return answer_weight;
};

void TSHwithPR::PathRelinking::DAG::Build(  const vector<int> &move_vertexes,const ColorSet &init_set,const ColorSet &guiding_set) {
    vector<vector<int>> directed_graph =  MakeDirectedGraph( move_vertexes , init_set , guiding_set );
    StronglyConnectedComponents SCC(directed_graph);
    dag_graph = SCC.build();
    dag_weight = MakeDagWeight(SCC, move_vertexes , init_set , guiding_set );
}

void TSHwithPR::PathRelinking::DAG::_test2() {
    ColorSet Sinitial,Sguiding;
    InputGraph();
    InputColorSet();
    Sinitial = init_color;
    Sguiding = init_color;
    // グラフ手計算に書いてあるカラーリングをメモ
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
    StronglyConnectedComponents SCC(graph);
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