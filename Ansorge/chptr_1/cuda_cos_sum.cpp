#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include "cuda_runtime.h"
#include "thrust/device_vector.h"

__host__ __device__ inline float cos_sum(float x , int terms){


// using Taylor series expansion of cos(x)
    float term = 1;
    float sum  = term;
    float x_squared = x * x;
    for ( int i = 1 ; i < terms ; i++){

        term *= -x_squared/(float)((2*i)*(2*i - 1));
        sum +=term;

    }
    return sum;
}
// GPU cosine integral sum using thread linear addressing
__global__ void cos_sum_gpu(float *sums, int steps,
                            int terms , float step_size)
{
    int step = blockIdx.x*blockDim.x+threadIdx.x;
    while( step < steps){

        float x = step_size*step;
        sums[step] = cos_sum(x, terms);
        step += gridDim.x*blockDim.x;
    }

                            

}

int main( int argc , char *argv[])
{

    int steps = (argc > 1)? atoi(argv[1]): 10e7;
    int terms = (argc > 2)? atoi(argv[2]):1000;
    int threads = (argc > 3)? atoi(argv[3]):256;
    int blocks = (argc > 4)? atoi(argv[4]):
                            (steps + threads-1)/threads;

    double pi = 3.1415926535897932384626433832795;
    double step_size = (pi)/(steps -1);

    thrust:: device_vector<float>device_sums(steps);
    float *device_ptr = thrust:: raw_pointer_cast(
                                &device_sums[0]);

    auto start = std::chrono::high_resolution_clock::now();
    cos_sum_gpu<<< blocks,threads>>>(device_ptr,steps,terms
                                    (float)step_size);
    double gpu_sum =
                    thrust::reduce(device_sums.begin(),
                                    device_sums.end());
    
    double gpu_time = std::chrono::duration<double>(
                                        std::chrono::high_resolution_clock::
                                        now() - start).count();
//Trapezoidal Rule correction
gpu_sum -= (cos_sum(0, terms) + cos_sum(pi, terms)) / 2.0f;
gpu_sum *= step_size;

    printf("GPU cosine Sum: %.10f\n", gpu_sum);
    printf("GPU elapsed time: %.10f seconds\n", gpu_time);

    return 0;
}