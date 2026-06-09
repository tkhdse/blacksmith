#include <pybind11/pybind11.h>

namespace py = pybind11;

int lower_fx(int i, int j) {
    return i + j;
}

PYBIND11_MODULE(blacksmith_, m, py::mod_gil_not_used()) {
    m.doc() = "pybind11 blacksmith plugin";
    m.def("lower_fx", &lower_fx, "A function that adds two numbers");
}