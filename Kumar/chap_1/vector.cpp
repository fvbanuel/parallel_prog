#include <omp.h>
#include <vector>
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