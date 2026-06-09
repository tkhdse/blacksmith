import blacksmith_
import torch
import torch.nn as nn


# blacksmith API

def compile(model: nn.Module):
    module = torch.export.export(model(), (torch.randn(100, 8), ))
    fx_out = module.run_decompositions().graph
    ret = blacksmith_.parse_fx(1,2)
    print(ret)
    print(fx_out)