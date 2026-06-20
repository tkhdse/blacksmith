#include "fuse.h"

void Fuser::runSegmentationPass() { 
    // at input time, the graph is guaranteed to have N fuseGroups, where each fuseGroup contains 1 of N nodes. 
    FuseGroup* cur = this->graph->getEntrypoint();
    
    while (cur) {
        FuseGroup* nextNode = cur->getNeighbors()[0]; // make this more scalable later -> bad code
        vector<FCOp*> cur_ops = cur->getOperators();
        vector<FCOp*> next_ops = nextNode->getOperators();

        if (next_ops.size() > 0) {
            FCOp* op = next_ops[0];
            if (cur->checkLegalFuse(op)) {
                cur->lowerGroup(nextNode->getOperatorClass());

                // merge cur and next   
                string to_evict = nextNode->getFusedName();
                cur->mergeGroups(nextNode);

                unordered_map<string, FuseGroup*>& fuse_groups = this->graph->getFuseGroups();

                // cout << "Evicting... " << to_evict << endl;
                fuse_groups.erase(to_evict);
                
                
                fuse_groups.erase(cur->fuse_head);
                cur->fuse_head = cur->getFusedName();
                fuse_groups.insert({cur->getFusedName(), cur});
                // cout << this->graph->getFuseGroups().size() << endl;
            }
        }
        
        vector<FuseGroup*> neighborGroups = cur->getNeighbors();

        if (neighborGroups.size() == 0) {
            cur = nullptr;
        }
    }
}


void Fuser::printFuseResults() {

    FuseGroup* fg = this->graph->getEntrypoint();
    while (fg) {
        string isEntry = (fg == graph->getEntrypoint()) ? "(entry) " : "";
        cout << isEntry << "Group: " << fg->getId() << ' ' << fg->getFusedName() << ' ' << '[' << getOperatorClassString(fg->getOperatorClass()) << ']' << " ";

        const auto& nbrGroups = fg->getNeighbors();
        if (nbrGroups.size() > 0) {
            cout << "Next: " << fg->getNeighbors()[0]->getFusedName() << endl;
            fg = fg->getNeighbors()[0];
        } else {
            cout << "Next: None" << endl;
            // fg = nullptr;
            break;
        }
    }
    cout << '\n';
}