#include "cuda_runtime.h"
#include "thrust/device_vector.h"
#include <stdlib.h>

__global__ void gpumult(float * __restrict C,
                        const float * __restrict A,
                        const float * __restrict B,
                        int Ay , int Ax, int Bx)
                        {
                            int tx = blockIdx.x*blockDim.x + threadIdx.x;
                            int ty = blockIdx.y*blockDim.y + threadIdx.y;

                            if (ty >= Ay || tx >= Bx) return;
                            C[ty*Bx +tx] = 0.0;

                            for (int k = 0;k < Ax;k++){
                                C[ty*Bx +tx] +=
                                                A[ty*Bx +k]*B[k*Bx + tx];

                            }

                    
                        }
                        int main( int argc , char *argv[])
                        {
                            return 0;
                        }



