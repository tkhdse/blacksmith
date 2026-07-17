#pragma once
#include <string>

using namespace std;

class KernelBuilder {
public:
    string emitKernelCode(string kernelName);
    void writeMTLFile();    

private:
    string source;
};

// string emitCode();
// void writeToFile();

// void writeMTLFile();