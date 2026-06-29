BUILD_DIR		:= build

METAL_CPP_DIR 	:= ../../../metal-cpp
KERNEL_SRC 		:= invoke.cpp
KERNEL_OBJ 		:= kernel_test_invoke.o
KERNEL_BIN 		:= $(BUILD_DIR)/kernel_test_invoke.o

METAL_CXXFLAGS := -O3 -Wall -std=c++17 \
	-I$(METAL_CPP_DIR) \
	-DNS_PRIVATE_IMPLEMENTATION \
	-DMTL_PRIVATE_IMPLEMENTATION \
	-DCA_PRIVATE_IMPLEMENTATION

METAL_LDFLAGS 	:= -framework Metal -framework Foundation -framework QuartzCore

CXXFLAGS	:= -O3 -Wall -std=c++17 -fPIC 
LDFLAGS		:= -shared -undefined dynamic_lookup

DEPS        := $(OBJS:.o=.d)

.PHONY: all clean # kernel_test

all: $(KERNEL_BIN)
	@echo "kernel_test built successfully"

$(KERNEL_BIN): $(KERNEL_SRC) | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(KERNEL_OBJ) $(METAL_LDFLAGS) -o $@

$(KERNEL_OBJ): $(KERNEL_SRC) | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(METAL_CXXFLAGS) -c $< -o $@

	
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(OUT_DIR)/$(MODULE)*.so