#include "gen.h"

#include <iostream>
#include <cstdio>

string emitCode() {
    return "#include <iostream>\n\nint main(int argc, char** argv) {\nstd::cout << \"Hello, world!\" << std::endl;\n}";
}


string emitKernelCode(string kernelName) {
    const string header = "kernel void " + kernelName + "(device const float* A, device const float* B, device float* C, uint index [[thread_position_in_grid]])";
    const string body = "\n\toutput[index] = A[index] + B[index];\n";
    return header + "{" + body + "}";
}

void writeToFile() {
    FILE* codeFile = fopen("tmp/source.cpp", "w");
    if (!codeFile) cerr << "File not found: tmp/source.cpp" << endl;

    const string codeString = emitCode();
    fprintf(codeFile, "%s", codeString.c_str());
    fclose(codeFile);
}

void writeMTLFile() {
    FILE* cgenKernelFile = fopen("tmp/source.metal", "w");
    if (!cgenKernelFile) cerr << "File not found: tmp/source.metal" << endl;

    const string codeString = emitKernelCode("add_");
    fprintf(cgenKernelFile, "%s", codeString.c_str());
    fclose(cgenKernelFile);
}