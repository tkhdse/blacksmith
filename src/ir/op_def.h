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

inline bool isLegalFuse(OperatorClass a, OperatorClass b) {
    if (a == opOpaque || b == opOpaque) return false;
    bool ret = (a == opInjective && b == opInjective) || (a == opInjective && b == opReduction) || (a == opCOF && b == opInjective);
    return ret;
}


// To Do:
class LatticeValue {
public:
    OperatorClass cls;
    LatticeValue(OperatorClass cls) {
        this->cls = cls;
    }

    OperatorClass meet(const LatticeValue& other) const {
        // include comparison/dominance logic
        return opInjective;
    }
};


string getOperatorClassString(OperatorClass op_class);


// FCOp -> "Fuse Candidate" Op. This must carry forward information about the fusion pattern.
class FCOp : public FXNode {
public:
    virtual ~FCOp() = 0;
    
    // for debugging
    void printInfo();

    vector<FCOp*> getNeighbors();
    void appendNeighbor(FCOp* op);
    OperatorClass getOpClass() { return op_class_; }

protected:
    explicit FCOp(string name, string target, OperatorClass op_class) : FXNode(name, target), op_class_(op_class) {}

private:
    const OperatorClass op_class_;
    vector<FCOp*> neighbors = {};
    vector<string> tensor_deps = {}; // temporary string -> promote to TensorDep object
};


class FCAddMMOp : public FCOp {
public:
    FCAddMMOp(const FXNode& fx) : FCOp(fx.name, fx.target, opCOF) {
        this->name = "addmm";
    }
};

// class FCAddOp : public FCOp {
// public:
//     FCAddOp(const FXNode& fx) : FCOp(fx.name, fx.target, opInjective) {
//         this->name = "add";
//     }
// };

// // linalg matrix multiply
// class FCMatMulOp: public FCOp {
// public:
//     FCMatMulOp(const FXNode& fx) : FCOp(fx.name, fx.target, opCOF) {
//         this->name = "matmul";
//     }
// };


class FCPermuteOp : public FCOp {
public:
    FCPermuteOp(const FXNode& fx) : FCOp(fx.name, fx.target, opInjective) {
        this->name = "permute";
    }
};


class FCSumOp : public FCOp {
public:
    FCSumOp(const FXNode& fx) : FCOp(fx.name, fx.target, opReduction) {
        this->name = "sum";
    }
};


class FCReLUOp : public FCOp {
public:
    FCReLUOp(const FXNode& fx) : FCOp(fx.name, fx.target, opInjective) {
        this->name = "relu";
    }
};

class FCSumIntList : public FCOp {
public:
    FCSumIntList(const FXNode& fx) : FCOp(fx.name, fx.target, opReduction) {
        this->name = "sumIntList";
    }
};