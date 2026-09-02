#include "cuda_runtime.h"
#include "thrust/device_vector.h"
#include <stdlib.h>
#include <random>

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

    // break

}
