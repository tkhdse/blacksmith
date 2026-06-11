#pragma once
#include <vector>
#include <string>

#include "../ir/node_def.h"
#include "../ir/op_def.h"

using namespace std;


class FusionGraph {

public:
    FusionGraph() {
        this->nodes = {};
    }

    ~FusionGraph() {
        for (auto& node : this->nodes) {
            delete node;
        }
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

// these nodes are used to compose our FusionGraph
FCOp* allocateFCNodeFromTarget(string& target);