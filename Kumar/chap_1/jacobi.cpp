#include <vector>
#include <omp.h>

// Jacobi iteration

std::vector<std::vector<double>> jacobi(const std::vector<std::vector<double>>& A)
{
    int cols = A[0].size();
    int rows = A.size();
    std::vector<std::vector<double>> A_1 = A;

    for(int i = 1; i < rows-1; i++ ){
        for( int j = 1; j < cols-1; j++){

            A_1[i][j] = 0.25*
            (A[i][j+1]+ A[i][j-1]+A[i-1][j]+A[i+1][j]);
    }

}

return A_1;
} 

// jacobi optimised by parallelisation of the outer loop and SIMD vectorisation of inner loop
void jacobi_simd(const std::vector<double>& A, std::vector<double>& A_1, int rows, int cols) {
    #pragma omp parallel for
    for (int i = 1; i < rows - 1; i++) {
        #pragma omp simd
        for (int j = 1; j < cols - 1; j++) {
            // Indexing of A[i][j] for flat array A_1:[i * cols + j]
            A_1[i * cols + j] = 0.25 * (A[i * cols + (j + 1)] + 
                                         A[i * cols + (j - 1)] + 
                                         A[(i - 1) * cols + j] + 
                                         A[(i + 1) * cols + j]);
        }
    }
}