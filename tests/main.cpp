#include <iostream>
#include "../src/TSHwithPR.hpp"
#include "../src/struct.hpp"

int main(){
    TSHwithPR test;
    test.InputGraph();
    test.InputColorSet();
    //test._GraphShow();
    test._ReassignTest();
}