import blacksmith_
import torch
import torch.nn as nn
from dataclasses import dataclass
from typing import Any, Tuple, Union

from blacksmith.ir import parse_fx


@dataclass
class BlacksmithModel:
    ret: int
    input_shape: torch.Size
    def print_model_info(self):
        print(f"Model compiled with return value: {self.ret}\n  Takes input with dims: {self.input_shape}")




# blacksmith API
def compile(model: nn.Module, 
            tensor_input: Union[torch.Tensor, Tuple[Any,...]]
) -> int:

    exported = torch.export.export(model, (tensor_input, ))
    fx_out = exported.run_decompositions().graph
    print(fx_out)

    parsed = parse_fx(fx_out)
    ret = blacksmith_.lower_fx(parsed)

    return BlacksmithModel(ret=ret, input_shape=tensor_input.shape)