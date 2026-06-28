PYTHON 		?= ./venv/bin/python3
CXX			?= c++
SRC_DIR		:= src
# FUSE_DIR	:= $(SRC_DIR)/fuser
# UTILS_DIR 	:= $(SRC_DIR)/utils
# IR_DIR 		:= $(SRC_DIR)/ir


OUT_DIR		:= python
BUILD_DIR	:= build

MODULE 		:= blacksmith_
EXT_SUFFIX	:= $(shell $(PYTHON) -m pybind11 --extension-suffix)
INCLUDES	:= $(shell $(PYTHON) -m pybind11 --includes)
TARGET		:= $(OUT_DIR)/$(MODULE)$(EXT_SUFFIX)

MAIN_SRC	:= $(SRC_DIR)/blacksmith.cpp
FUSE_SRCS	:= $(wildcard $(FUSE_DIR)/*.cpp)
UTILS_SRCS 	:= $(wildcard $(UTILS_DIR)/*.cpp)
IR_SRCS 	:= $(wildcard $(IR_DIR)/*.cpp)

# SRCS		:= $(MAIN_SRC) $(FUSE_SRCS) $(UTILS_SRCS) $(IR_SRCS)
# OBJS 		:= $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
SRCS        := $(shell find $(SRC_DIR) -name '*.cpp' -o -name '*.cc')
OBJS        := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,\
               $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(SRCS)))


METAL_CPP_DIR 	:= metal-cpp
KERNEL_SRC 		:= $(SRC_DIR)/metal/kernels/invoke.cpp
KERNEL_OBJ 		:= $(BUILD_DIR)/metal/kernels/kernel_test_invoke.o
KERNEL_BIN 		:= $(BUILD_DIR)/kernel_test

METAL_CXXFLAGS := -O3 -Wall -std=c++17 \
	-I$(METAL_CPP_DIR) \
	-DNS_PRIVATE_IMPLEMENTATION \
	-DMTL_PRIVATE_IMPLEMENTATION \
	-DCA_PRIVATE_IMPLEMENTATION

METAL_LDFLAGS 	:= -framework Metal -framework Foundation -framework QuartzCore

CXXFLAGS	:= -O3 -Wall -std=c++17 -fPIC 
LDFLAGS		:= -shared -undefined dynamic_lookup

DEPS        := $(OBJS:.o=.d)

.PHONY: all clean kernel_test

kernel_test: $(KERNEL_BIN)
	@echo "kernel_test built successfully"

$(KERNEL_BIN): $(KERNEL_SRC) | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(KERNEL_OBJ) $(METAL_LDFLAGS) -o $@

$(KERNEL_OBJ): $(KERNEL_SRC) | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(METAL_CXXFLAGS) -c $< -o $@



all: $(TARGET)
	@echo "Blacksmith built successfully"

$(TARGET): $(OBJS) | $(OUT_DIR)
	$(CXX) $(LDFLAGS) $(OBJS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR) $(OUT_DIR):
	mkdir -p $@

	
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(OUT_DIR)/$(MODULE)*.so


-include $(DEPS)