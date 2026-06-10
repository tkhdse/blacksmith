#pragma once
#include "node_def.h"

#include <iostream>

/*
Define ops based on Aten/TVM's operator set: 
* https://github.com/apache/tvm/tree/main/src/relax/op
* https://docs.pytorch.org/docs/2.12/user_guide/torch_compiler/torch.compiler_ir.html


add      -> pointwise
ReLU     -> pointwise
sum      -> reduction
conv2d   -> complex-out-fusable
... 

Establish mappings between operator and class (injective, reduction, complex-out-fusable, opaque) for FusionAnalyzer
*/ 

enum OperatorClass {
    opInjective = 0, // injective
    opReduction = 1, // reduction   
    opCOF = 2,       // complex-out-fusable
    opOpaque = 3,   //  opaque
};



// FCOp -> "Fuse Candidate" Op. This must carry forward information about the fusion pattern.
class FCOp : public FXNode {
public:
    virtual ~FCOp() = 0;

    explicit FCOp(OperatorClass op_class) : op_class_(op_class) {}
    
    // for debugging
    void printInfo() {
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

private:
    const OperatorClass op_class_; 
};


FCOp::~FCOp() = default;

class FCAddOp : FCOp {
public:
    FCAddOp() : FCOp(opInjective) {}
};


class FCSumOp : FCOp {
public:
    FCSumOp() : FCOp(opReduction) {}
};


class FCReLUOp : FCOp {
public:
    FCReLUOp() : FCOp(opInjective) {}
};