#include "parse.h"
#include <iostream>
#include <sstream>

// expects an input to the permute op: e.g '[1,0]' -> {1,0}
vector<int> parseVector(string stringEntries) {
    int size = stringEntries.size();
    if (size == 0 || (stringEntries[0] != '[' && stringEntries[size-1] != ']')) {
        return {};
    }

    vector<int> out;   
    int cur = 0;

    for (int i = 1; i < size-1; ++i) {
        if (stringEntries[i] == ' ') continue;
    
        if (stringEntries[i] == ',') {
            out.push_back(cur);
            cur = 0;
        } else {
            cur = cur * 10 + (stringEntries[i] - '0');
        }
    }

    out.push_back(cur);
    cout << to_string(out[0]) << ',' << to_string(out[1]) << endl;

    return out;
}   

void printVector(vector<int> vec) {
    std::ostringstream oss;

    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i < vec.size() - 1) {
            oss << ", "; 
        }
    }

    cout <<  "<" + oss.str() + ">" << endl;
}