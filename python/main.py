import blacksmith
import torch
import numpy as np

import torch.nn as nn
import torch.nn.functional as F



class Model(nn.Module):
    def __init__(self):
        super().__init__()
        self.fc1 = nn.Linear(8, 16)
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(16, 4)
    
    def forward(self, x):
        x = self.relu(self.fc1(x))
        return self.fc2(x)


if __name__ == "__main__":
    model = Model()

    # takes an [W x 8] input
    inp = torch.randn(1000, 8)
    ret = blacksmith.compile(model, tensor_input=inp)
    ret.print_model_info()