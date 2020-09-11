#pragma once
#include "util.hpp"
#include "TSHwithPR.hpp"

class AnalizerForTSH : TSHwithPR {
    public:
        static void SaveLearningCurve(int itr,std::string filename);
        static void ShowLearningCurve();
        static std::tuple<double,long long, long long> CalcDistanceAndScoreInEliteSet();
        static void Input4Train();
        static void Input4Train(std::string graph_name, std::string color_name,std::string log_name);
        static void Train(std::string log_name);
        static void Analize();
        static void Eval();
};