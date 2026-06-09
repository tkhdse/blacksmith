#pragma once

#include <vector>
#include <string>


using namespace std;

class FXNode {
    public:
        string name;
        string op_name;
        string target;
        vector<string> args;
    
        vector<int> shape;
        string dtype;
        int index = -1;
    };