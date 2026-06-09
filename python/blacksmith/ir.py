import torch
from dataclasses import dataclass
from typing import List, Optional

@dataclass
class Node:
    name: str
    op_name: str
    target: str
    args: List[str]

    shape: Optional[List[int]] = None
    dtype: Optional[str] = None
    index: Optional[int] = None



def parse_fx(graph: torch.fx.Graph):
    adj = {}
    
    flattened_nodes = []
    for index, node in enumerate(graph.nodes):

        # print(node.name, node.op, node.target)
        cur = None
        if node.op == "output":
            cur = Node(
                node.name,
                node.op,
                str(node.target),
                [str(arg) for arg in node.args],
                None,
                None,
                index
            )
        else:
            val = node.meta.get('val')
            shape = list(val.shape) # tuple -> array
            dtype = str(val.dtype)

            cur = Node(
                node.name,
                node.op,
                str(node.target),
                [str(arg) for arg in node.args],
                shape,
                dtype,
                index
            )
        
        flattened_nodes.append(cur)

    return flattened_nodes