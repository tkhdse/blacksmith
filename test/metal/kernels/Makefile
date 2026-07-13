BUILD_DIR		:= build

METAL_CPP_DIR 	:= ../../../metal-cpp
KERNEL_SRC 		:= invoke.cpp
KERNEL_OBJ 		:= kernel_test_invoke.o
KERNEL_BIN 		:= $(BUILD_DIR)/kernel_test_invoke.o


# SHADER_SRCS		:= $(wildcard *.metal)
SHADER_SRCS 	:= add.metal
SHADER_AIRS		:= $(SHADER_SRCS:%.metal=$(BUILD_DIR)/%.air)
SHADER_LIBS		:= $(SHADER_SRCS:%.metal=$(BUILD_DIR)/%.metallib)

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


all: $(KERNEL_BIN) $(SHADER_LIBS)
	@echo "kernel_test built successfully"

$(BUILD_DIR):
	mkdir -p $@

$(KERNEL_BIN): $(KERNEL_OBJ) | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(KERNEL_OBJ) $(METAL_LDFLAGS) -o $@

$(KERNEL_OBJ): $(KERNEL_SRC) | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(METAL_CXXFLAGS) -c $< -o $@


$(BUILD_DIR)/%.air: %.metal | $(BUILD_DIR)
	xcrun -sdk macosx metal -c $< -o $@

$(BUILD_DIR)/%.metallib: $(BUILD_DIR)/%.air
	xcrun -sdk macosx metallib $< -o $@
	
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(OUT_DIR)/$(MODULE)*.so