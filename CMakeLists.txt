cmake_minimum_required(VERSION 3.20)
project(Blacksmith CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT APPLE)
    message(FATAL_ERROR "Blacksmith requires macOS (Metal).")
endif()

# --- metal-cpp submodule ---
set(METAL_CPP_DIR "${CMAKE_SOURCE_DIR}/metal-cpp")
if(NOT EXISTS "${METAL_CPP_DIR}/Metal/Metal.hpp")
    message(FATAL_ERROR
        "metal-cpp not found. Run: git submodule update --init --recursive")
endif()

# --- Apple frameworks ---
find_library(METAL_LIB      Metal      REQUIRED)
find_library(FOUNDATION_LIB Foundation REQUIRED)
find_library(QUARTZCORE_LIB QuartzCore)  # optional unless you use QuartzCore

# --- sources (mirrors Makefile find) ---
file(GLOB_RECURSE BLACKSMITH_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# --- pybind11 from venv (must use venv path, not resolved framework python) ---
set(BLACKSMITH_VENV_PYTHON "${CMAKE_SOURCE_DIR}/venv/bin/python3")
if(NOT EXISTS "${BLACKSMITH_VENV_PYTHON}")
    message(FATAL_ERROR "venv not found at ${BLACKSMITH_VENV_PYTHON}. Run: python3 -m venv venv && pip install -r python/requirements.txt")
endif()

execute_process(
    COMMAND "${BLACKSMITH_VENV_PYTHON}" -m pybind11 --cmakedir
    OUTPUT_VARIABLE pybind11_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY
)
find_package(pybind11 CONFIG REQUIRED)

# Python headers/libs for the extension (can use venv hint)
set(Python_FIND_VIRTUALENV FIRST)
find_package(Python COMPONENTS Interpreter Development REQUIRED)

pybind11_add_module(blacksmith_ ${BLACKSMITH_SOURCES})

target_include_directories(blacksmith_ PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${METAL_CPP_DIR}"
)

target_link_libraries(blacksmith_ PRIVATE
    ${METAL_LIB}
    ${FOUNDATION_LIB}
)

if(QUARTZCORE_LIB)
    target_link_libraries(blacksmith_ PRIVATE ${QUARTZCORE_LIB})
endif()

# metal-cpp symbol implementations — exactly ONE .cpp file
set(METAL_CPP_IMPL_CPP "${CMAKE_SOURCE_DIR}/src/metal/kernels/invoke.cpp")
set_source_files_properties("${METAL_CPP_IMPL_CPP}" PROPERTIES
    COMPILE_DEFINITIONS
        "NS_PRIVATE_IMPLEMENTATION;MTL_PRIVATE_IMPLEMENTATION"
)

# pybind11 extension on macOS (same as Makefile -undefined dynamic_lookup)
target_link_options(blacksmith_ PRIVATE "-undefined" "dynamic_lookup")

# write .so next to python package (same as Makefile OUT_DIR)
set_target_properties(blacksmith_ PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/python"
)