#include <iostream>
#include "../src/TSHwithPR.hpp"
#include "../src/util.hpp"
#include "../src/FullSearch.hpp"
using namespace std;
int main(){
    FullSearch test;
    test.InputGraph();
    test.InputColorSet();
    test.Calc();
    test._ShowColorSet(test.current_color); 
    cout << "Eval:" << test.EvalFunction(test.current_color) << endl;
}