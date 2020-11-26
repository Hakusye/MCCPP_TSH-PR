#include "../include/MCCPP.hpp"
#include "../include/util.hpp"
using namespace std;

ColorSet MCCPP::current_color;
ColorSet MCCPP::reset_color;
//win10だと実行の度に同じ乱数になるバグが発生
MCCPP::MCCPP() {
    mt.seed(rd());
}

MCCPP::~MCCPP() {

}
//テストクリア(ほぼ確実に正しい)
long long MCCPP::EvalFunction(const ColorSet &color_set) {
    float sum = 0;
    vector<ColorClass> target_color_class = color_set.GetColorSet();
    for(ColorClass target : target_color_class) {
        sum += target.weight * target.vertexes.size();
        for(int vertex : target.vertexes) { //その色の中で隣接があるなら数分M足す
            set<int> neighbor_vertexes = AdjacentColorSet(target.vertexes,vertex);
            sum +=  neighbor_vertexes.size() * M / 2.0;
        }
    }
    return sum; 
}

void MCCPP::_ShowGraph() {
    vector<ColorClass> target_color_class = init_color.GetColorSet();
    for(int i=0;i<target_color_class.size();i++){
        cout << target_color_class[i].weight << " ";
    }
    cout << endl;
    for(int i=0;i<graph.size();i++) {
        cout << "vertex" << i << ":";
        for(int j=0;j<graph[i].size();j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
}

void MCCPP::_ShowColorSet(const ColorSet &color_set) {
    cout << "score:" << color_set.score  << endl;
    vector<ColorClass> target_color_class = color_set.GetColorSet();
    for(ColorClass color_class : target_color_class) {
        if(color_class.vertexes.size() == 0) continue;
        for(int vertex : color_class.vertexes) { 
            cout << vertex << " ";
        }
        cout << endl;
    }
}

void MCCPP::InputGraph() {
    string filename;
    ifstream inputfile;
    bool ok;
    do {
        ok = true;
        cout << "グラフのファイル名を入力してください" << endl;
        cin >> filename;
        inputfile.open(filename);
        if(inputfile.fail()) {
            cout << "Error:ファイルにアクセス出来ません" << endl;
            ok = false;
        }
    } while(!ok);
    InitGraph(inputfile);
}

void MCCPP::InputGraph(const string &filename) {
    ifstream inputfile;
    inputfile.open(filename);
    if(inputfile.fail()) {
        cout << "Error:ファイルにアクセス出来ません" << endl;
        exit(1);
    }
    InitGraph(inputfile);
}

void MCCPP::InitGraph(ifstream& inputfile) {
    string line;
    while(getline(inputfile,line)) {
        vector<string> words = split(line, ' ');
        //入力データの形式に合わせる
        if(words[0] == "c") continue;
        else if(words[0] == "p") {
            graph.resize(stoi(words[2])); //余剰あってもいいかも
            num_edges = stoi(words[3]);
        }else if(words[0] == "e") {
            int a = stoi(words[1])-1; //頂点を0からにした
            int b = stoi(words[2])-1;
            graph[a].push_back(b);
            graph[b].push_back(a);
        }
    }
}

void MCCPP::InputColorSet() {
    string filename;
    ifstream inputfile;
    bool ok;
    do {
        ok = true;
        cout << "カラークラスのファイル名を入力してください" << endl;
        cin >> filename;
        inputfile.open(filename);
        if(inputfile.fail()) {
            cout << "Error:ファイルにアクセス出来ません" << endl;
            ok = false;
        }
    }while(!ok);
    InitColorSet(inputfile);
}

void MCCPP::InputColorSet(const string &filename) {
    ifstream inputfile;
    inputfile.open(filename);
    if(inputfile.fail()) {
        cout << "Error:ファイルにアクセス出来ません" << endl;
        exit(1);
    }
    InitColorSet(inputfile);
}

void MCCPP::InitColorSet(ifstream& inputfile) {
    vector<ColorClass> color_class_set;
    string line;
    while(getline(inputfile,line)) {
        ColorClass C;
        C.weight = stod(line);
        color_class_set.push_back(C);
    }
    init_color.InitColorSet(color_class_set);
    current_color.InitColorSet(init_color);
}

//テストクリア(テストケース..._LargestAdjacentTest)
pair<int, set<int>> MCCPP::LargestAdjacentVertexInSet(const set<int> &candidate_set,const set<int> &graph_sub_set) {
    set<int> max_adjacent_set = {};
    int max_vertex = -1; int max_adjacent = -1;
    // uncolord_setの中でUともっとも隣接している頂点を選択
    for(int vertex : candidate_set) {
        set<int> tmp_adjacent_set = AdjacentColorSet(graph_sub_set, vertex);
        int tmp_size = tmp_adjacent_set.size();
        if( max_adjacent < tmp_size ) {
            max_adjacent_set = tmp_adjacent_set;
            max_vertex = vertex;
            max_adjacent = tmp_adjacent_set.size();
        }
    }
    return {max_vertex , max_adjacent_set};
}

set<int> MCCPP::OldAdjacentColorSet(const set<int> &vertexes_set,const int &target_vertex) {
    vector<int> g_adjasent_set = graph[target_vertex];//頂点vertexの隣合う頂点はグラフから確認できる
    set<int> ans_set;
    for (int vertex : g_adjasent_set) {
        if( vertex == target_vertex ) continue;
        if(vertexes_set.find(vertex) != vertexes_set.end()) ans_set.insert(vertex);
    }
    return ans_set;
}
//今だけNewAdjacentColorSetを入れ替え
set<int> MCCPP::AdjacentColorSet(const set<int> &vertexes_set,const int &target_vertex) {
    vector<int> g_adjasent_set = graph[target_vertex];//頂点vertexの隣合う頂点はグラフから確認できる
    set<int> tmp_set(g_adjasent_set.begin(),g_adjasent_set.end());
    return IntersectionSet2Set(vertexes_set,tmp_set);
}

//テストクリア(テストケース... _ReassignLargestCardinality )
long long MCCPP::DiffEvalFunction(const ColorSet &color_set,const int &change_vertex,const int &after_color) {
    long long ans = color_set.score;
    int pre_color = color_set.GetSearchColor().at(change_vertex);
    vector<ColorClass> target_color_class =  color_set.GetColorSet();
    ans -= target_color_class[pre_color].weight;
    set<int> adjacent_color = AdjacentColorSet(target_color_class[pre_color].vertexes,change_vertex);
    ans -= adjacent_color.size() * M; // 隣接頂点が同じだった数だけ引く

    ans += target_color_class[after_color].weight;
    adjacent_color = AdjacentColorSet(target_color_class[after_color].vertexes,change_vertex);
    ans += adjacent_color.size() * M; // 隣接頂点の色が同じになった数だけ増やす

    return ans;
}

//テストクリア(テストケース... _RemoveSetTest)
set<int> MCCPP::RemoveSet2Set(set<int> main_set,const set<int> &remove_set) {
    for ( int remove_element : remove_set) {
        if( main_set.find(remove_element) != main_set.end() ) main_set.erase( remove_element );
    }
    return main_set;
}
set<int> MCCPP::IntersectionSet2Set(const set<int> left_set,const set<int> right_set) {
    set<int> ans_set;
    set_intersection( //initial[i]とguiding[target_color]の積集合
        left_set.begin(),left_set.end(),
        right_set.begin(),right_set.end(),
        inserter(ans_set,ans_set.end())
    );
    return ans_set;
}
//まだ
void MCCPP::Run() {

}

void MCCPP::ShowResult() {

}

void MCCPP::SavePath() {

}