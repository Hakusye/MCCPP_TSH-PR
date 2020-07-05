#pragma once
#include <set>
#include <string>
#include <vector>
#include <map>

struct ColorClass {
    std::string color;
    std::set<int> vertexes;
    double weight;
};

struct ColorSet {
    std::vector<ColorClass> S;
    std::map<int,int> search_color; //vartex number,color number 
    int num_color;
    double score;
};

struct VertexMove {
    int vertex;
    int color;
};