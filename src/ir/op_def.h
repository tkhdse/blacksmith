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
    
    // for debugging
    void printInfo();

protected:
    explicit FCOp(string name, string target, OperatorClass op_class) : FXNode(name, target), op_class_(op_class) {}

private:
    const OperatorClass op_class_;
    vector<FCOp*> neighbors = {};
};


class FCAddOp : public FCOp {
public:
    FCAddOp(const FXNode& fx) : FCOp(fx.name, fx.target, opInjective) {}
};


class FCPermuteOp : public FCOp {
public:
    FCPermuteOp(const FXNode& fx) : FCOp(fx.name, fx.target, opInjective) {}
};


class FCSumOp : public FCOp {
public:
    FCSumOp(const FXNode& fx) : FCOp(fx.name, fx.target, opReduction) {}
};


class FCReLUOp : public FCOp {
public:
    FCReLUOp(const FXNode& fx) : FCOp(fx.name, fx.target, opInjective) {}
};