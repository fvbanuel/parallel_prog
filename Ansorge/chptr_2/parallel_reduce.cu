#include <stdlib.h>
#include <bit>
#include "cuda_runtime.h"
#include "thrust/device_vector.h"

__global__ void reduce(float *y , float *x, int n)
{
    extern__shared float sum[];

    int id = threadIdx.x;
    int tid = blockDim.x*blockIdx.x+threadIdx.x;
    int stride = gridDim.x*blockDim.x;
    sum[id] = 0.0f;
    for(int i =tid; i < n;i+=stride)
    {
        sum[id] += x[i];
    }
    __syncthreads();

    int block = std::bit_ceil(blockDim.x);

    for(int i = block/2; i >0; i >>= 1)
    {
        if (id < i && id + i < blockDim.x)
        {
            sum[id] += sum[i+k];
        }
        __syncthreads();
    }

    if (id == 0) y[blockIdx.x] = sum[0];
    
    
}