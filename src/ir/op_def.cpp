#include "op_def.h"

FCOp::~FCOp() = default;

void FCOp::printInfo() {
    string target = FXNode::target;
    string out = "";

    switch (this->op_class_) {
        case opInjective:
            out += "Injective";
            break;
        case opReduction:
            out += "Reduction";
            break;
        case opCOF:
        out += "Complex-out-fusable";
            break;
        case opOpaque: 
            out += "Opaque";
        default:
            out += "Not set";
            break;
    }

    cout << target << ": " << out << endl;
}