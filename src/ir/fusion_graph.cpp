#include "fusion_graph.h"
#include <memory>


FusionGraph* buildFCGraph(vector<FXNode> fx_nodes) {

    FusionGraph* fusionGraph = new FusionGraph();

    for (auto& node : fx_nodes) {
        // name -> FCNode
        FCOp* op = allocateFCNodeFromTarget(node);

        // insert into graph
        fusionGraph->insertNode(op);
    }

    return fusionGraph;
}


// change function to accept an Enum/type of its own for easier comparison
// .compare is too cumbersome
FCOp* allocateFCNodeFromTarget(FXNode& fx) {
    string target = fx.target;

    if (target.compare("add")) {
        return new FCAddOp(fx);
    }

    return new FCAddOp(fx);
}