#pragma once
#include <set>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <utility>
#include <random>
#include <queue>
#include <iomanip>
#include <time.h>

struct ColorClass {
    std::string color;
    std::set<int> vertexes;
    long long weight;
    bool operator< (const ColorClass &another_color_class) const;
    bool operator> (const ColorClass &another_color_class) const;
};
// ColorSet内の関数でないと中身を変更できない仕様にする(カプセル化する)
struct ColorSet {
  private:
    //std::vector<ColorClass> S;
    //std::map<int, int> search_color; // vartex number,color number
  public:
  // 普段はprivate
    std::vector<ColorClass> S;
    std::map<int, int> search_color; // vartex number,color number
    int num_color;
    long long score;
    bool operator<(const ColorSet &another_color_set) const;
    bool operator>(const ColorSet &another_color_set) const;
    bool operator==(const ColorSet &another_color_set) const;
    void operator=(const ColorSet &copy_color_set);
    std::vector<ColorClass> GetColorSet() const;
    std::map< int, int > GetSearchColor() const;
    void GreaterSort();
    void MoveVertexColor( int pre_vertex,int after_color );
    void InitColorSet(const ColorSet copy_color_set);
    void InitColorSet(const std::vector<ColorClass> copy_color_set);
    void SortColorWeight();
    void ShowColor(int pre_color,int vertex);
};

struct VertexMove {
    int vertex;
    int color;
    bool operator> (const VertexMove &another_vertex_move) const;
    bool operator< (const VertexMove &another_vertex_move) const;
};
//used priority_queue
std::vector<int> shuffle(std::vector<int> target,std::mt19937 random_device);
std::vector<std::string> split(std::string str,char del);
bool pair_set_int_comp (const std::pair<std::set<int>,int> &left,const std::pair<std::set<int>,int> &right);
bool pair_set_int_comp_greater (const std::pair<std::set<int>,int> &left,const std::pair<std::set<int>,int> &right);
bool set_comp (const std::set<int> &left,const std::set<int> &right);
bool set_comp_greater (const std::set<int> &left,const std::set<int> &right);
bool set_int_equal (const std::set<int> &left,const std::set<int> &right);

