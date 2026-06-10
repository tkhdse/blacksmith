#pragma once

#include <vector>
#include <string>

using namespace std;


// FXNode -> simplified C++ based representation for PyTorch FX Nodes (from blacksmith Ppython library).
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
