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
    float weight;
    bool operator< (const ColorClass &another_color_class) const;
    bool operator> (const ColorClass &another_color_class) const;
};

struct ColorSet {
    std::vector<ColorClass> S;
    std::map<int,int> search_color; //vartex number,color number 
    int num_color;
    float score;
    bool operator< (const ColorSet &another_color_set) const;
    bool operator> (const ColorSet &another_color_set) const;
    bool operator== (const ColorSet &another_color_set) const;
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

