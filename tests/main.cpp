#include <iostream>
#include "../src/TSHwithPR.hpp"
#include "../src/util.hpp"
#include "../src/FullSearch.hpp"
using namespace std;

int main(){
    TSHwithPR test;
    test.InputGraph();
    test.InputColorSet();
    test._ReassignTest();
    test._ShowColorSet(test.current_color);
}