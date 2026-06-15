#include "gen.h"

#include <iostream>
#include <cstdio>

string emitCode() {
    return "#include <iostream>\n\nint main(int argc, char** argv) {\nstd::cout << \"Hello, world!\" << std::endl;\n}";
}

void writeToFile() {
    FILE* codeFile = fopen("tmp/source.cpp", "w");
    if (!codeFile) cerr << "File not found: tmp/source.cpp" << endl;

    const string codeString = emitCode();
    fprintf(codeFile, "%s", codeString.c_str());
    fclose(codeFile);
}