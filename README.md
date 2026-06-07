# Blacksmith: a Compute Graph Operation Fuser for Metal

The blacksmith compiler builds on my previous work on [soptRT](https://github.com/tkhdse/soptRT). Blacksmith is designed to perform fusion for Metal kernels so that I can program on my new Mac GPU. 


```
make

# optional: create and activate virtual environment
pip install -r requirements.txt
. venv/bin/activate

python3 python/main.py
```