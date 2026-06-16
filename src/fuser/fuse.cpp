#include "fuse.h"

void Fuser::runSegmentationPass() { 

    // Borrowed logic from old buildFCGraph():
    // // fuse logic
    // if (fg->checkLegalFuse(op)) {
    //     fg->lowerGroup(op->getOpClass());
        
    // } else {
    // fg = graph->createNewFuseGroup();
    // }
}


void Fuser::printFuseResults() {
    for (auto& fg : graph->getGroups()) {
        string isEntry =  fg == graph->getEntrypoint() ? "(entry) " : "";
        cout << isEntry << "Group: " << fg->getId() << ' ' << fg->getFusedName() << ' ' << '[' << getOperatorClassString(fg->getOperatorClass()) << ']' << endl;
        for (auto& fcop : fg->getOperators()) {
            fcop->printInfo();
        }
    }
}