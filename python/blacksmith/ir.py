import torch
import blacksmith_
# from dataclasses import dataclass
# from typing import List, Optional

# # @dataclass
# # class Node:
# #     name: str
# #     op_name: str
# #     target: str
# #     args: List[str]

# #     shape: Optional[List[int]] = None
# #     dtype: Optional[str] = None
# #     index: Optional[int] = None



def parse_fx(graph: torch.fx.Graph):
    
    flattened_nodes = []
    for index, node in enumerate(graph.nodes):
        fx = blacksmith_.FXNode()
        fx.name = node.name
        fx.op_name = node.op
        fx.target = str(node.target)
        fx.args = [str(arg) for arg in node.args]
        fx.index = index

        if node.op == "output":
            fx.shape = []
            fx.dtype = ""
        else:
            val = node.meta.get('val')
            fx.shape = list(val.shape) # tuple -> array
            fx.dtype = str(val.dtype)
    
        # print(fx.name, ':',fx.args)
        flattened_nodes.append(fx)

    return flattened_nodes