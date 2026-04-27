cmake_minimum_required(VERSION 3.18)
project(ParallelProgrammingRoot LANGUAGES CXX CUDA)


set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_ARCHITECTURES 80)


find_package(OpenMP REQUIRED)
find_package(CUDAToolkit REQUIRED)

add_subdirectory(Ansorge)
add_subdirectory(Kumar)
