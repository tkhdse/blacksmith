# Blacksmith: a Compute Graph Operation Fuser for Metal

The Blacksmith compiler builds on my previous work on [soptRT](https://github.com/tkhdse/soptRT). Blacksmith is designed to perform fusion for Metal kernels so that I can program on my new Mac GPU. 

Blacksmith's design stems from thorough research between 3 prominent players in Deep Learning Compilers: TVM, nvFuser, and TorchInductor. The 3 differ architecturally, and Blacksmith aims to draw inspiration from the best parts of each design to form a usable Metal compiler. 



## Setup
Compile JIT compiler code for IR generation and transformation passes. 
```
make
```

[Optional: create and activate virtual environment]
```
pip install -r requirements.txt
. venv/bin/activate
```

Execute script containing `blacksmith.compile(MODEL)`:
```
python3 python/main.py
```  


## Implementation
Blacksmith leverages PyTorch's TorchDynamo to generate an FX Graph (compute graph) for a given model definition. Blacksmith lowers this high-level compute graph to it's own IR containing a `FusionGraph` and `FCNOps` ("Fusion-candidate" ops). 

FX Graph for the sample model defined in `python/main.py`:
![sample dag](docs/sample_DAG.jpg)

The architecture makes use of the following fusion passes and lowering for correct execution:
* Fusion Analysis
* Segmentation
* Scheduling
* Codegen


Kernel dispatch and orchestration. Options for implementation include:
* VM bytecode and execution as used in TVM.
* Source C++ file generation as used in TorchInductor.


## To do: 
* Code Generator for Metal kernels
* Debugger [Compute Graph Visualization, Emit graph]
* Dynamic Shape Approach (currently starting with Static shape support for simplicity)
* Explicit API (compiled_model = blacksmith.compile(model))