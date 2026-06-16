#include "fuse.h"

void Fuser::runSegmentationPass() { 

}


void Fuser::printFuseResults() {
    for (auto& fg : graph->getGroups()) {
        cout << "Group: " << fg->getId() << ' ' << '[' << getOperatorClassString(fg->getOperatorClass()) << ']' << endl;
        for (auto& fcop : fg->getOperators()) {
            fcop->printInfo();
        }
    }
}