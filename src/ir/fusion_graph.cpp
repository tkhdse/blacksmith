#include "fusion_graph.h"
#include "target_def.h"
#include <memory>


FusionGraph* buildFCGraph(vector<FXNode> fx_nodes) {

    FusionGraph* fg = new FusionGraph();

    for (auto& node : fx_nodes) {

        auto it = fg->name2op.find(node.name);
        if (it != fg->name2op.end()) {
            return nullptr;
        }

        if (node.op_name.compare("call_function") == 0) {
            
            // target -> FCNode
            FCOp* op = allocateFCNodeFromTarget(node);
            
            // insert into graph
            fg->insertNode(op);

            // store name->Op for later retrieval
            fg->name2op[node.name] = op;

            for (auto& dep : node.args) {
                if (fg->name2op.find(dep) != fg->name2op.end()) {
                    fg->name2op[dep]->appendNeighbor(op);
                }
            }
        }
    }

    return fg;
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

    default: 
        cerr << "Unsupported Operator: " << '[' << fx.target << ']' << endl;
    }

    return new FCAddOp(fx);
}