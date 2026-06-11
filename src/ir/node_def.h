#pragma once

#include <vector>
#include <string>

using namespace std;


// FXNode -> simplified C++ based representation for PyTorch FX Nodes (from blacksmith Ppython library).
class FXNode {
public:
    FXNode() = default;

    string name;
    string op_name;
    string target;
    vector<string> args;

    vector<int> shape;
    string dtype;
    int index = -1;

protected:
    explicit FXNode(const string name, const string target) {
        this->name = name;
        this->target = target;
    }
};
