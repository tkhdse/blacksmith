Test Metal kernel invocations:


The Makefile handles building `.metal` files as well as the files containing the `metal-cpp` API.

```
cd metal/kernels
make
./build/kernel_test_invoke.o [kernel_name] # e.g add_, mm_
```
