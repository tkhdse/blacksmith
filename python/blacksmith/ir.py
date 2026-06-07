import torch

def parse_fx(graph: torch.fx.Graph):
    print(graph)