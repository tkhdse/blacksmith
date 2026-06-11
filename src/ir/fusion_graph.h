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

    unordered_map<string, FCOp*> name2op; // -> op_name -> FCOp
    // unordered_map<string, FCOp*> dataDeps; // -> op_name -> FCOp
    

private:
    // store all FCNodes
    vector<FCOp*> nodes;
    // unordered_map<> placeholders; // placeholder_name -> dependency


    // entrypoints
};


FusionGraph* buildFCGraph(vector<FXNode> fx_nodes);

// these nodes are used to compose our FusionGraph
FCOp* allocateFCNodeFromTarget(FXNode& target);