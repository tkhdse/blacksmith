#pragma once
#include "node_def.h"

#include <iostream>
#include "../utils/parse.h"

using namespace std;

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

    // abstract function: inferShape -> each operator must define its own shape result
    virtual vector<int> inferShape(vector<vector<int>> arg_shapes) = 0;


    
    protected:
    explicit FCOp(const FXNode& fx, OperatorClass op_class, string op_string) : FXNode(fx), op_class_(op_class), op_string(std::move(op_string)) {}
    
    private:
    const OperatorClass op_class_;
    const string op_string;
    vector<FCOp*> neighbors = {};
    vector<string> tensor_deps = {}; // temporary string -> promote to TensorDep object
};


class FCAddMMOp : public FCOp {
public:
    FCAddMMOp(const FXNode& fx) : FCOp(fx, opCOF, "addmm") {}

    vector<int> inferShape(vector<vector<int>> arg_shapes) override {
        return {};
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
    FCPermuteOp(const FXNode& fx) : FCOp(fx, opInjective, "permute") {}

    vector<int> inferShape(vector<vector<int>> arg_shapes) override {

        // cout << "permiute: " << this->args.size() << endl;
        // for (auto& arg : this->args) {
        //     cout << "permute: " <<  arg << endl;
        //     if (arg[0] == '[') {
        //         vector<int> parsedArg = parseVector(arg);
        //         printVector(parsedArg);
        //     }
        // }

        // for (auto& arg : arg_shapes) {

        // }

        return {};
    }


};


class FCSumOp : public FCOp {
public:
    FCSumOp(const FXNode& fx) : FCOp(fx, opReduction, "sum") {}

    vector<int> inferShape(vector<vector<int>> arg_shapes) override {
        return {};
    }
};


class FCReLUOp : public FCOp {
public:
    FCReLUOp(const FXNode& fx) : FCOp(fx, opInjective, "relu") {}

    vector<int> inferShape(vector<vector<int>> arg_shapes) override {
        return {};
    }

};

class FCSumIntList : public FCOp {
public:
    FCSumIntList(const FXNode& fx) : FCOp(fx, opReduction, "sumIntList") {}

    vector<int> inferShape(vector<vector<int>> arg_shapes) override {
        return {};
    }
};