#include <pybind11/pybind11.h>

namespace py = pybind11;

int parse_fx(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(blacksmith_, m, py::mod_gil_not_used()) {
    m.doc() = "pybind11 example plugin";
    m.def("parse_fx", &parse_fx, "A function that adds two numbers");
}