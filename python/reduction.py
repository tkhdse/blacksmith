import blacksmith
import torch
import torch.nn as nn


@blacksmith.compile
class Model(nn.Module):
    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(8, 16)
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(16, 4)
        self.fc_post = nn.Linear(1, 4)   # runs after reduction
        self.relu_post = nn.ReLU()
    def forward(self, x):
        # Group A: injective stack (like main.py)
        x = self.relu(self.fc1(x))       # [100, 16]  addmm + relu
        x = self.fc2(x)                  # [100, 4]   addmm
        # Reduction boundary
        x = x.sum(dim=1, keepdim=True)   # [100, 1]   sum (reduction)
        # Group B: injective ops after reduction
        x = self.relu_post(self.fc_post(x))  # addmm + relu
        return x