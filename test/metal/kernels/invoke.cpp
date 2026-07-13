// #define NS_PRIVATE_IMPLEMENTATION
// #define MTL_PRIVATE_IMPLEMENTATION
// #define CA_PRIVATE_IMPLEMENTATION

#include <Metal/Metal.hpp>
#include <Foundation/Foundation.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <iostream>
#include <time.h>


const int num_peek = 5;

const int arrsize = 1024;
const int buffsize = arrsize * sizeof(int);

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "expected 2 or fewer arguments" << std::endl;
        return 1;
    }

    char* kernel_name;
    if (argc == 2) {
        kernel_name = argv[1];
    } else {
        kernel_name = "mm_";
    }

    srand(time(NULL)); // random seed based on current time

    std::cout << "Executing kernel... " << kernel_name << std::endl;

    MTL::Device* device = MTL::CreateSystemDefaultDevice();

    // MTL::Library* lib = device->newDefaultLibrary();
    NS::Error* liberr = nullptr;
    MTL::Library* lib = device->newLibrary(NS::String::string("build/add.metallib", NS::UTF8StringEncoding),  &liberr);

    if (!lib) {
        std::cerr << "failed to create: lib" << std::endl;
        return 1;
    }

    MTL::Function* func = lib->newFunction(NS::String::string(kernel_name, NS::ASCIIStringEncoding));

    if (!func) {
        std::cerr << "failed to link: func" << std::endl;
        return 1;
    }

    NS::Error* err = nullptr;
    MTL::ComputePipelineState* pipeline_state = device->newComputePipelineState(func, &err);

    MTL::CommandQueue* cq = device->newCommandQueue();
    MTL::CommandBuffer* cbuff = cq->commandBuffer();
    MTL::ComputeCommandEncoder* encoder = cbuff->computeCommandEncoder();

    // encode the kernel and buffers
    MTL::Buffer* buff_a = device->newBuffer(buffsize, MTL::ResourceStorageModeShared);
    MTL::Buffer* buff_b = device->newBuffer(buffsize, MTL::ResourceStorageModeShared);
    MTL::Buffer* buff_c = device->newBuffer(buffsize, MTL::ResourceStorageModeShared);

    encoder->setComputePipelineState(pipeline_state);

    // populate buffers
    encoder->setBuffer(buff_a, 0, 0);
    encoder->setBuffer(buff_b, 0, 1);
    encoder->setBuffer(buff_c, 0, 2);

    // generate random int data
    int* a = (int*)buff_a->contents();
    int* b = (int*)buff_b->contents();
    for (unsigned i = 0; i < arrsize; ++i) {
        a[i] = rand() % 100;
        b[i] = rand() % 200;
    }

    std::cout << "Buffer A: [";
    for (unsigned i = 0; i < num_peek; ++i) {
        std::cout << a[i] << ' ';
    }
    std::cout << "]" << std::endl;
    
    std::cout << "Buffer B: [";
    for (unsigned i = 0; i < num_peek; ++i) {
        std::cout << b[i] << ' ';
    }
    std::cout << "]" << std::endl;



    // dispatch kernel
    MTL::Size grid_size = MTL::Size(arrsize, 1, 1);
    NS::UInteger tg_size_ = pipeline_state->maxTotalThreadsPerThreadgroup();// thread_group_size
    if (tg_size_ > arrsize) tg_size_ = arrsize;

    MTL::Size tg_size = MTL::Size(tg_size_, 1, 1);

    encoder->dispatchThreads(grid_size, tg_size);


    // clean up
    encoder->endEncoding();
    cbuff->commit();
    cbuff->waitUntilCompleted();


    std::cout << "invoked test metal pipeline: " << std::endl;
    int* c = (int*)buff_c->contents();
    std::cout << "Buffer C: [";
    for (unsigned i = 0; i < num_peek; ++i) {
        std::cout << c[i] << ' ';
    }
    std::cout << "]" << std::endl;

    return 0;
}