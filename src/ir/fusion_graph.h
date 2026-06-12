#pragma once
#include <vector>
#include <string>

#include "../ir/node_def.h"
#include "../ir/op_def.h"

using namespace std;


class FuseGroup {
public: 
    FuseGroup() {}

    ~FuseGroup() {
        for (auto& node : this->nodes) {
            delete node;
        }
    }


    // checkLegalFuse(node) -> check node's OperatorClass against the current FuseGroup
    //  true -> add node to current FuseGroup and promote FuseGroup class if needed
    //  false -> split and make a new FuseGroup

    // To do: verify OperatorClass against legal transitions
    bool checkLegalFuse(FCOp* op);

    void addToGroup(FCOp* op);

    // lowers the current operator class based on defined hierarchy
    //  injective -> reduction -> complex-out-fuse -> opaque
    void lowerGroup(OperatorClass cls);

private:
    vector<FCOp*> nodes;
    OperatorClass curr_op_class = opInjective; // injective by default (highest precedence)
};


class FusionGraph {

public:
    FusionGraph() {
        this->fuse_groups = {};
    }

    ~FusionGraph() {
        for (auto& fg : this->fuse_groups) {
            delete fg;
        }        
    }

    vector<FuseGroup*> getGroups() const {
        return this->fuse_groups;
    }

    // void insertNode(FCOp* op) {
    //     this->nodes.push_back(op);
    // }

    FuseGroup* createNewFuseGroup() {
        FuseGroup* nfg = new FuseGroup;
        this->fuse_groups.push_back(nfg);
        return nfg;
    }

    unordered_map<string, FCOp*> name2op; // -> op_name -> FCOp
    // unordered_map<string, FCOp*> dataDeps; // -> op_name -> FCOp
    

private:
    // store all FCNodes
    FuseGroup* entrypoint;
    vector<FuseGroup*> fuse_groups;
    // unordered_map<> placeholders; // placeholder_name -> dependency


    // entrypoints
};


FusionGraph* buildFCGraph(vector<FXNode> fx_nodes);

// these nodes are used to compose our FusionGraph
FCOp* allocateFCNodeFromTarget(FXNode& target);