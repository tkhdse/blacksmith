#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <string>
#include <vector>
#include <list>
#include <variant>
#include <iostream>

#include "utils/fx_utils.h"
#include "utils/node_def.h"


namespace py = pybind11;
using namespace std;



int lower_fx(const py::list& fx_graph) {
    cout << "entered" << endl;

    vector<FXNode> fx_nodes;

    for (auto& item : fx_graph) {
        FXNode fx_node = item.cast<FXNode>();
        fx_nodes.push_back(fx_node);
    }

    print_nodes(fx_nodes);

    // vector<FXNode*> nodes = fx_graph.cast<vector<FXNode*>>();
    return 0;
}

PYBIND11_MODULE(blacksmith_, m, py::mod_gil_not_used()) {
    m.doc() = "pybind11 blacksmith plugin";

    py::class_<FXNode>(m, "FXNode")
        .def(py::init<>())
        .def_readwrite("name", &FXNode::name)
        .def_readwrite("op_name", &FXNode::op_name)
        .def_readwrite("target", &FXNode::target)
        .def_readwrite("args", &FXNode::args)
        .def_readwrite("shape", &FXNode::shape)
        .def_readwrite("dtype", &FXNode::dtype)
        .def_readwrite("index", &FXNode::index);

    m.def("lower_fx", &lower_fx, "Performs compilation steps for fusion and lowering to Metal");
}