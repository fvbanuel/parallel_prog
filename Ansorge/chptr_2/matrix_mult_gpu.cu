#include "cuda_runtime.h"
#include "thrust/device_vector.h"
#include <stdlib.h>
#include <random>
// A is Ay*Ax, B is Ax*Bx, C is Ay*Bx
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
                            int Arow = (argc > 1) ? atoi(argv[1]) : 1024;
                            int Acol = (argc > 2) ? atoi(argv[2]) : Arow;
                            int Brow = Acol;
                            int Bcol = (argc > 3)? atoi(argv[3]):Brow;
                            int Crow = Arow;
                            int Ccol = Bcol;

                            uint tilex = (argc > 4)? atoi(argv[5]): 32;
                            uint tiley = (argc > 5)? atoi(argv[6]): 8;

                            thrust::host_vector<float> A(Arow*Acol);
                            thrust::host_vector<float> B(Brow*Bcol);
                            thrust::host_vector<float> C(Crow*Ccol);
                            thrust::device_vector<float> d_A(Arow*Acol);
                            thrust::device_vector<float> d_B(Brow*Bcol);
                            thrust::device_vector<float> d_C(Crow*Ccol);

                            std::default_random_engine gen(1111101);
                            std::uniform_real_distribution<float> dis(0.0, 1.0);
                            for (int i = 0; i < Arow*Acol; i++)
                                A[i] = dis(gen);
                            for (int i = 0; i < Brow*Bcol; i++)
                                B[i] = dis(gen);
                            // host to device copy
                            d_A = A; 
                            d_B = B;

                            dim3 threads = {tilex, tiley, 1};

                            dim3 blocks = {(Bcol + threads.x -1)/threads.x,
                                            (Arow+threads.y -1)/threads.y, 1};

                            gpumult<<<blocks, threads>>>(thrust::raw_pointer_cast(d_C.data()),
                                                    thrust::raw_pointer_cast(d_A.data()),
                                                    thrust::raw_pointer_cast(d_B.data()),
                                                    Arow, Acol, Bcol);
                            cudaDeviceSynchronize();


                            return 0;
                        }



