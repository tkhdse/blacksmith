#pragma once
#include <vector>
#include <string>

#include "../ir/node_def.h"
#include "../ir/op_def.h"
#include "../ir/tensor_node_def.h"

using namespace std;


class FuseGroup {
public: 
    FuseGroup(const int id) {
        this->group_id = id;
        this->fused_kernel_name = "";
    }

    ~FuseGroup() {
        for (auto& node : this->nodes) {
            delete node;
        }
    }

    vector<FCOp*>& getOperators() {
        return this->nodes;
    }

    int getId() {
        return this->group_id;
    }

    string getFusedName() {
        return this->fused_kernel_name;
    }


    OperatorClass getOperatorClass() {
        return curr_op_class;
    }

    void addNeighbor(FuseGroup* new_fg) {
        nextGroups.push_back(new_fg);
    }

    vector<FuseGroup*>& getNeighbors() {
        return nextGroups;
    }


    // checkLegalFuse(node) -> check node's OperatorClass against the current FuseGroup
    //  true -> add node to current FuseGroup and promote FuseGroup class if needed
    //  false -> split and make a new FuseGroup

    // To do: verify OperatorClass against legal transitions
    bool checkLegalFuse(FCOp* op);

    void mergeGroups(FuseGroup* fg);

    void addToGroup(FCOp* op);

    // lowers the current operator class based on defined hierarchy
    //  injective -> reduction -> complex-out-fuse -> opaque
    void lowerGroup(OperatorClass cls);

    string fuse_head;
private:
    vector<FCOp*> nodes = {};
    vector<FuseGroup*> nextGroups = {};

    OperatorClass curr_op_class = opInjective; // injective by default (highest precedence)
    int group_id;

    string fused_kernel_name;

    vector<int> in_shape;
    vector<int> out_shape;
};


class FusionGraph {

public:
    FusionGraph() {
        this->fuse_groups = {};
    }

    ~FusionGraph() {
        // delete allocated FuseGroups
        for (auto& pair : this->fuse_groups) {
            delete pair.second;
        }
        
        // delete allocated Tensors/Placeholders
        for (auto& pair : this->tensors) {
            delete pair.second; 
        }
    }

    unordered_map<string, FuseGroup*>& getFuseGroups() {
        return this->fuse_groups;
    }

    void addTensorToGraph(TensorNode* tn) {
        if (tensors.find(tn->name) == tensors.end()) {
            tensors.insert({tn->name, tn});
        }
    }

    FuseGroup* createNewFuseGroup() {
        FuseGroup* nfg = new FuseGroup(this->group_id);
        this->group_id += 1;
        return nfg;
    }

    bool isEntrypoint() { return group_id == 1; }
    FuseGroup* getEntrypoint() { return entrypoint; }
    void setEntrypoint(FuseGroup* fg) { entrypoint = fg; }

    unordered_map<string, FCOp*> name2op; // -> op_name -> FCOp

private:
    FuseGroup* entrypoint;
    unordered_map<string, FuseGroup*> fuse_groups;
    unordered_map<string, int> op2id; // implement new key strategy for fuseGroups
    unordered_map<string, TensorNode*> tensors;
    int group_id = 0;
};


FusionGraph* buildFCGraph(vector<FXNode> fx_nodes);

// these nodes are used to compose our FusionGraph
FCOp* allocateFCNodeFromTarget(FXNode& target);

TensorNode* allocateTensor(FXNode& fx);