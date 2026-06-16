#pragma once
#include "../ir/fusion_graph.h"

class Fuser {
public:
    Fuser(FusionGraph* fuse_graph) {
        this->graph = fuse_graph;
    }

    void runAnalysisPass();
    void runSegmentationPass();
    void runSchedulePass();
    void runCodegenPass();

    void printFuseResults();

private:
    FusionGraph* graph;
};