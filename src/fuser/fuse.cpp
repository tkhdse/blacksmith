#include "fuse.h"

void Fuser::runSegmentationPass() { 
    FuseGroup* entrypoint = this->graph->getEntrypoint();



    
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

        const auto& nbrGroups = fg->getNeighbors();
        if (nbrGroups.size() > 0) {
            cout << "Next: " << fg->getNeighbors()[0]->getFusedName() << endl;
        } else {
            cout << "Next: None" << endl;

        }
        // for (auto& fcop : fg->getOperators()) {
        //     fcop->printInfo();
        // }
    }
}