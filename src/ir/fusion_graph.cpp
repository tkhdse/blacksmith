#include "fusion_graph.h"
#include "target_def.h"
#include <memory>


FusionGraph* buildFCGraph(vector<FXNode> fx_nodes) {

    FusionGraph* graph = new FusionGraph();
    FuseGroup* fg;// = graph->createNewFuseGroup();

    for (auto& node : fx_nodes) {
        auto it = graph->name2op.find(node.name);
        if (it != graph->name2op.end()) {
            return nullptr;
        }

        if (node.op_name.compare("placeholder") == 0) {
            cout << node.name << ' ' <<  '(' << node.shape[0] << ',' << node.shape[1] << ')' << endl;
        }

        if (node.op_name.compare("call_function") == 0) {
            
            // target -> FCNode
            FCOp* op = allocateFCNodeFromTarget(node);
            
            fg = graph->createNewFuseGroup();
            if (graph->isEntrypoint()) {
                graph->setEntrypoint(fg);
            }
            
            // insert to group (can either be old or new FuseGroup)
            fg->addToGroup(op);

            // store name->Op for later retrieval
            graph->name2op[node.name] = op;

            for (auto& dep : node.args) { // an argument can either be a dependency node (FCOp) or a placeholder
                if (graph->name2op.find(dep) != graph->name2op.end()) {
                    graph->name2op[dep]->appendNeighbor(op);
                }
            }
        }
    }

    return graph;
}


// change function to accept an Enum/type of its own for easier comparison
// .compare is too cumbersome
FCOp* allocateFCNodeFromTarget(FXNode& fx) {
    AtenTarget target = parseTarget(fx.target);

    switch (target) {
    case AtenTarget::AddMM:
        return new FCAddOp(fx);
    case AtenTarget::Permute:
        return new FCPermuteOp(fx);
    case AtenTarget::ReLU:
        return new FCReLUOp(fx);
    case AtenTarget::SumIntList:
        return new FCSumIntList(fx);


    default: 
        cerr << "Unsupported Operator: " << '[' << fx.target << ']' << endl;
    }

    return new FCAddOp(fx);
}

TensorNode* allocateTensor(FXNode& fx) {
    return new TensorNode();
}


bool FuseGroup::checkLegalFuse(FCOp* op) {
    return isLegalFuse(curr_op_class, op->getOpClass()); 
}

void FuseGroup::addToGroup(FCOp* op) {
    this->fused_kernel_name += "_" + op->name;
    this->nodes.push_back(op);
}

void FuseGroup::lowerGroup(OperatorClass cls) {
    // To Do: upgrade this to use the LatticeValue class
    this->curr_op_class = max(this->curr_op_class, cls);
}