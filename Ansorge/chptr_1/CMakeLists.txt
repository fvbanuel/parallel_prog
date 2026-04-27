
add_executable(ch1_ex kernel_ch1.cu include/helper_ch1.h)


target_include_directories(ch1_ex PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_include_directories(ch1_ex PRIVATE ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})


