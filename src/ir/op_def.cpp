#include "op_def.h"
#include <vector>

FCOp::~FCOp() = default;

void FCOp::appendNeighbor(FCOp* op) {
    this->neighbors.push_back(op);
}

vector<FCOp*> FCOp::getNeighbors() {
    return this->neighbors;
}

void FCOp::printInfo() {
    string target = FXNode::target;
    string out = getOperatorClassString(this->op_class_);

    cout << target << ": " << out << endl;

    string vec = "";
    for (auto& op : neighbors) {
        vec += op->name + ", ";
    }

    cout << '\t' << "Next: " << '[' << vec << ']' << endl;
}

string getOperatorClassString(OperatorClass op_class) {
    string out = "";
    
    switch (op_class) {
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
    return out;
}