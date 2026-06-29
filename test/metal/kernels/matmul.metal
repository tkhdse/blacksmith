static int TILE_WIDTH = 16;

kernel void mm_(device const float* A, device const float* B, device float* C, uint A_Y, uint A_X, uint B_Y, uint index [[thread_position_in_grid]]) {
    
    // define tiles in shared memory
    threadgroup float tileA[TILE_WIDTH][TILE_WIDTH];
    threadgroup float tileB[TILE_WIDTH][TILE_WIDTH];
    
    // how does index map to a 2d space? 
    uint ty = index % TILE_WIDTH;
    uint tx = index / TILE_WIDTH;

    // populate tileA, tileB
    // iterates over tiles
    for (int i = 0; i < ceil(A_Y/TILE_WIDTH); i++) { // iterate over the shared dimension
        for (int j = 0; j < ceil(A_X/TILE_WIDTH); j++) {
            if (j*TILE_WIDTH + tx >= A_X) {
                tileA[][] = 0.0f;
            } else {
                tileA[][] = A[];
            }

            if (j*TILE_WIDTH + ty >= B_Y) {
                tileB[][] = 0.0f;
            } else {
                tileB[][] = B[];
            }
        }

        threadgroup_barrier(mem_flags::mem_threadgroup);
        for (int k = 0; k < TILE_WIDTH; k++) {
            C[] = tileA[k][tx] * tileB[ty][k]; 
        }
        threadgroup_barrier(mem_flags::mem_threadgroup);
    }
}