#include "cuda_runtime.h"
#include "thrust/device_vector.h"
#include <stdlib.h>
#include <random>
//kernel for tiled matrix multiplication
template< int TS> __global__ void gputiled(
    float * __restrict C, float * __restrict A,
    float * __restrict B, int Ay,int Ax, int Bx)
{
    __shared__ float Atile[TS][TS];
    __shared__ float Btile[TS][TS];

    int tx = threadIdx.x;
    int ty = threadIdx.y;
    int ocx = blockDim.x*blockIdx.x;
    int ocy = blockDim.y*blockIdx.y;

    int ax = tx;
    int ay = ocy+ty;
    int bx = ocx+tx;
    int by = ty;

    float csum = 0.0;
    for(int t = 0;t< gridDim.x; t++){

        Atile[ty][tx] = A[ay*Ax+ax];
        Btile[ty][tx] = B[by*Bx+bx];
        __syncthreads();
        for( int k =0; k<TS; k++){
            csum += Atile[ty][k]*Btile[k][tx];
        }
        __syncthreads();
        ax +=TS;
        by +=TS;
    }
    C[ay*Bx+bx] = csum;

}
