#include <omp.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

// parallelisation works well for big a.size()
float dot_product(const std::vector<float>& a, const std::vector<float>& b) {
    float result = 0.0f;
    #pragma omp parallel for simd reduction(+:result) 
    for (size_t i = 0; i < a.size(); ++i) {
        result += a[i] * b[i];
    }
    return result;
}
std::vector<float> hadamard_product(const std::vector<float>& a, const std::vector<float>& b) {
    std::vector<float> result(a.size());
    #pragma omp parallel for simd 
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] * b[i];
    }
    return result;

}

// prefix sum, 
std::vector<float> prefix_sum(const std::vector<float>& a) {
    std::vector<float> result(a.size());
    result[0] = a[0];
    #pragma omp parallel for ordered(1)
    for (size_t i = 1; i < a.size(); ++i)
    {   
        #pragma omp ordered depend( sink: i-1 )

        result[i] = result[i - 1] + a[i];
        #pragma omp ordered depend( source )
    }
    return result;
}

// add neighbouring blocks on top and right of (i,j)
void grid_computation(int N, int M, std::vector<std::vector<float>>& grid) {
    #pragma omp parallel for ordered(2)
    for (int i = 1; i < N; i++) {
        for (int j = 1; j < M; j++) {
            
           
            #pragma omp ordered depend(sink: i-1, j) depend(sink: i, j-1)
            
            grid[i][j] = grid[i-1][j] + grid[i][j-1];
            
            
            #pragma omp ordered depend(source)
        }
    }
}
void grid_computation_flat(int N , int M, std:: vector<float>& grid){

    // flat grid index for (i,j) = grid[i*M + j]

    #pragma omp parallel for ordered(2)
    for (int i = 1; i < N; i++) {
        for (int j = 1; j < M; j++) {
        
            #pragma omp ordered depend(sink: i-1, j) depend(sink: i, j-1)
        
        
            grid[i * M + j] = grid[(i-1) * M + j] + grid[i * M + (j-1)];
        
            #pragma omp ordered depend(source)
    }
}

}



