// #define NS_PRIVATE_IMPLEMENTATION
// #define MTL_PRIVATE_IMPLEMENTATION
// #define CA_PRIVATE_IMPLEMENTATION

#include <Metal/Metal.hpp>
#include <Foundation/Foundation.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <iostream>

int main() {
    MTL::Device* device = MTL::CreateSystemDefaultDevice();

    NS::Error* err = nullptr;
    MTL::Library* lib = device->newLibrary(NS::String::string("defaultLib", NS::UTF8StringEncoding), &err);

    if (!lib) {
        std::cerr << "failed to create: lib" << std::endl;
    }

    MTL::Function* func = lib->newFunction(NS::String::string("mm_", NS::UTF8StringEncoding)); //"mm_"

    if (!func) {
        std::cerr << "failed to link: func" << std::endl;
    }


    MTL::CommandQueue* cq = device->newCommandQueue();

    std::cout << "invoked test metal pipeline" << std::endl;

}
