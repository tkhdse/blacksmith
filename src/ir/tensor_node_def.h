#include "node_def.h"

class TensorNode : public FXNode {
public:
    TensorNode(FXNode& fx) : FXNode(fx) {}
};