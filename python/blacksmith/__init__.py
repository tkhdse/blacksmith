import blacksmith 
import torch
import torch.nn as nn

def compile(model: nn.Module):
    module = torch.export.export(model(), (torch.randn(100, 8), ))
    fx_out = module.run_decompositions().graph
    print(fx_out)
    # serialized_nodes = 
    # fx_graph = blacksmith.compile(serialized_nodes)
    # print(fx_graph)