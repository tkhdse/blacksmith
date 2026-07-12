// #define NS_PRIVATE_IMPLEMENTATION
// #define MTL_PRIVATE_IMPLEMENTATION
// #define CA_PRIVATE_IMPLEMENTATION

#include <Metal/Metal.hpp>
#include <Foundation/Foundation.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <iostream>

int main() {
    MTL::Device* device = MTL::CreateSystemDefaultDevice();
    MTL::Library* lib;
    MTL::Function* func;


    std::cout << "invoked test metal pipeline" << std::endl;

}
