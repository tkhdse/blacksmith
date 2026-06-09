#include "fx_utils.h"
#include <iostream>
using namespace std;

void print_nodes(vector<FXNode>& nodes) {
    for (const auto& node : nodes) {
        cout << node.index << ", " << node.name << ": " << node.op_name << ' ' << node.target << endl;
        cout << "\t args: ";
        for (const auto& arg : node.args) {
            cout << arg << ", ";
        }
        cout << endl;

        if (node.op_name.compare("output") != 0) {
            cout << "\t shape: (" << node.shape[0] << ", " << node.shape[1] << ')' << endl;
            cout << "\t dtype: " << node.dtype << endl;
        }
    }
}