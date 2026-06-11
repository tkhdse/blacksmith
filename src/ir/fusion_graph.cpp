#include "fusion_graph.h"
#include "target_def.h"
#include <memory>


FusionGraph* buildFCGraph(vector<FXNode> fx_nodes) {

    FusionGraph* fusionGraph = new FusionGraph();

    for (auto& node : fx_nodes) {

        if (node.op_name.compare("call_function") == 0) {
            
            // target -> FCNode
            FCOp* op = allocateFCNodeFromTarget(node);
            
            // insert into graph
            fusionGraph->insertNode(op);
        }


    }

    return fusionGraph;
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