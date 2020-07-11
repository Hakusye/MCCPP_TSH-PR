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
};

struct VertexMove {
    int vertex;
    int color;
};
//used priority_queue
bool operator< (const ColorSet &c1, const ColorSet &c2);
std::vector<std::string> split(std::string str,char del);
