#include "fuse.h"

void Fuser::runSegmentationPass() { 
    // at input time, the graph is guaranteed to have N fuseGroups, where each fuseGroup contains 1 of N nodes. 
    FuseGroup* cur = this->graph->getEntrypoint();
    
    while (cur) {
        vector<FuseGroup*> neighbors = cur->getNeighbors();

        if (neighbors.size() == 0) {
            cur = nullptr;
            break;
        } 

        FuseGroup* next = neighbors[0]; // make this more scalable later -> bad code

        // vector<FCOp*> cur_ops = cur->getOperators();
        vector<FCOp*> next_ops = next->getOperators();

        FCOp* op = next_ops[0];

        if (cur->checkLegalFuse(op)) {
            cur->lowerGroup(next->getOperatorClass());

            // merge cur and next   
            int nextId = cur->mergeGroups(next);

            // obtain reference to fuse_groups
            unordered_map<string, FuseGroup*>& fuse_groups = this->graph->getFuseGroups();
            
            string evictKey = "group_" + to_string(nextId);
            fuse_groups.erase(evictKey);
        } else {
            cur = neighbors.size() > 0 ? neighbors[0] : nullptr;
        }
    }
}


void Fuser::printFuseResults(string title) {
    cout << title << endl;

    FuseGroup* fg = this->graph->getEntrypoint();
    while (fg) {
        string isEntry = (fg == graph->getEntrypoint()) ? "(entry) " : "";
        cout << isEntry << "Group: " << fg->getId() << ' ' << fg->getFusedName() << ' ' << '[' << getOperatorClassString(fg->getOperatorClass()) << ']' << " ";


        string shape_string = "Shape: <";

        vector<FCOp*>& fg_operators = fg->getOperators();
        // get last operator
        int last = fg_operators.size()-1;
        FCOp* fg_shape_op = fg_operators[last];
        vector<int> fg_shape = fg_shape_op->shape;

        string dim_info = "";
        for (auto& dim : fg_shape) {
            dim_info += (to_string(dim) + ',');
        }

        if (dim_info.size() > 0){
            dim_info[dim_info.size()-1] = '>';
            shape_string += dim_info;
        } else {
            shape_string += ">";
        }
        cout << shape_string << " ";


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