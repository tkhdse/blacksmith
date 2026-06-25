static int TILE_WIDTH = 16;

kernel void mm_(device const float* A, device const float* B, device float* C, uint A_Y, uint A_X, uint B_Y, uint index [[thread_position_in_grid]]) {
    
    // define tiles in shared memory
    threadgroup float tileA[TILE_WIDTH * TILE_WIDTH];
    threadgroup float tileB[TILE_WIDTH * TILE_WIDTH];
    
    // how does index map to a 2d space? 
    // uint y = 
    // uint x = 

    // populate tileA, tileB
    
    // iterates over tiles
    for (int i = 0; i < ceil(A_X/TILE_WIDTH); i++) {
        if (i*TILE_WIDTH + x >= A_X) {
            tileA[][] = 0.0f;
        } else {
            tileA[][] = A[];
        }
    }

    for (int i = 0; i < ceil(B_Y/TILE_WIDTH); i++) {
        if (i*TILE_WIDTH + y >= B_Y) {
            tileB[][] = 0.0f;
        } else {
            tileB[][] = B[];
        }
    }

    threadgroup_barrier(mem_flags::mem_threadgroup);
    for (int i = 0; i < TILE_WIDTH; i++) {
        C[y][x] = tileA[i][x] * tileB[y][i]; 
    }
    threadgroup_barrier(mem_flags::mem_threadgroup);
}