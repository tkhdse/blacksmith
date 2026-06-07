PYTHON 		?= ./venv/bin/python3
CXX`		?= c++
SRC			:= src/blacksmith.cpp
OUT_DIR		:= python
MODULE 		:= blacksmith_
EXT_SUFFIX	:= $(shell $(PYTHON) -m pybind11 --extension-suffix)
INCLUDES	:= $(shell $(PYTHON) -m pybind11 --includes)
TARGET		:= $(OUT_DIR)/$(MODULE)$(EXT_SUFFIX)

CXXFLAGS	:= -O3 -Wall -shared -std=c++11 -fPIC -undefined dynamic_lookup

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) -o $(TARGET)

clean:
	rm -f $(OUT_DIR)/$(MODULE)*.so