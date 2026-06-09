import blacksmith_
import torch
import torch.nn as nn

from blacksmith.ir import parse_fx


# blacksmith API

def compile(model: nn.Module):
    module = torch.export.export(model(), (torch.randn(100, 8), ))
    fx_out = module.run_decompositions().graph

    parsed = parse_fx(fx_out)
    print(parsed)
    
    ret = blacksmith_.lower_fx(1,2)
    print(ret)