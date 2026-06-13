#include "target_def.h"
#include <unordered_map>


// for comparison efficiency, map target to Enum type
AtenTarget parseTarget(const string& target) {
    static unordered_map<string, AtenTarget> mappings = {
        {"aten.permute.default",    AtenTarget::Permute},
        {"aten.addmm.default",      AtenTarget::AddMM},
        {"aten.relu.default",       AtenTarget::ReLU},
        {"aten.sum.dim_IntList",    AtenTarget::SumIntList},
    };


    auto it = mappings.find(target);
    if (it != mappings.end()) {
        return it->second;
    }

    return AtenTarget::None;
}