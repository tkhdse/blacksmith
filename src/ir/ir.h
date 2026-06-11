#pragma once
#include <vector>
#include <string>

#include "../utils/node_def.h"
#include "../utils/op_def.h"

using namespace std;


class FusionGraph {

public:
    FusionGraph() {
        this->nodes = {};
    }

    vector<FCOp*> getNodes() const {
        return this->nodes;
    }

    void insertNode(FCOp* op) {
        this->nodes.push_back(op);
    }


private:
    // store all FCNodes
    vector<FCOp*> nodes;
    // entrypoints
};


FusionGraph* buildFCGraph(vector<FXNode> fx_nodes);
FCOp* getFCNodeFromTarget(string& target);