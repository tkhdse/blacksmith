#pragma once

#include <string>
using namespace std;

// register operators in this enum
enum class AtenTarget {
    None = 0,
    Add,
    MatMul,
    Permute,
    ReLU,
    SumIntList
};

AtenTarget parseTarget(const string& target);