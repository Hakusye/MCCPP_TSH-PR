
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <utility>
#include "TSHwithPR.hpp"
#include "struct.hpp"
using namespace std;

ColorSet MCCPP::current_color;

MCCPP::MCCPP() {

}

MCCPP::~MCCPP() {

}

vector<string> split(string str,char del) {
    int first = 0;
    int last = str.find_first_of(del);
    vector<string> ans;
    while(first < str.size()) {
        string word(str, first, last - first);
        ans.push_back(word);
        first = last + 1;
        last = str.find_first_of(del,first);
        if(last == string::npos) last = str.size();
    }
    return ans;
}
//テストクリア
double MCCPP::EvalFunction(const ColorSet color_set) {
    double sum = 0;
    for(ColorClass target : color_set.S) {
        sum += target.weight * target.vertexes.size();
        for(int vertex : target.vertexes) {
            set<int> neighbor_vertexes = AdjacentColorSet(target.vertexes,vertex);
            for(int neighbor_vertex : neighbor_vertexes) {
                if(color_set.search_color.at(vertex) == color_set.search_color.at(neighbor_vertex)) {
                    sum += M;
                }
            }
        }
    }
    return sum/2.0; 
}

void MCCPP::_ShowGraph() {
    for(int i=0;i<init_color.S.size();i++){
        cout << init_color.S[i].weight << " ";
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

void MCCPP::_ShowColorSet(ColorSet color_set) {
    cout << "score:" << color_set.score  << endl;
    for(ColorClass color_class : color_set.S) {
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
    string line;
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
    string line;
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

    while(getline(inputfile,line)) {
        ColorClass C;
        C.weight = stod(line);
        init_color.S.push_back(C);
    }
    current_color = init_color;
    cout << "ok" << endl;
}
//まだ
void MCCPP::Run() {

}

void MCCPP::ShowResult() {

}

void MCCPP::SavePath() {

}
//テストクリア(LargestAdjacentVErtexInSetに内包しているため)
set<int> MCCPP::AdjacentColorSet(set<int> vertexes_set,int vertex) {
    vector<int> g_adjasent_set = graph[vertex];//頂点vertexの隣合う頂点はグラフから確認できる
    set<int> ans_set;
    for (int vertex : g_adjasent_set) {
        if(vertexes_set.find(vertex) != vertexes_set.end()) ans_set.insert(vertex);
    }
    return ans_set;
}