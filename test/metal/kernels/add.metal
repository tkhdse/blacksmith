#include <metal_stdlib>

kernel void add_(device const int* A, device const int* B, device int* C, uint index [[thread_position_in_grid]]){
    // metal::os_log user_log("add_", "kernel_debug");
    // user_log.log_info("thread started...");
	C[index] = A[index] + B[index];
}