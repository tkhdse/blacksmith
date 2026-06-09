#pragma once

enum OperatorClass {
    opOpaque = 0,
};

OperatorClass getOperatorClass() {
    // perform operator class lookup based on regisrtration (register.h)
    return opOpaque;
}