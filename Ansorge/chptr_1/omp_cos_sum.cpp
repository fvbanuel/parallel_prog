#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <omp.h>

// cosine integral sum using OpenMP

inline float cos_sum(float x , int terms){

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

int main(int argc, char *argv[]){

    int steps = (argc >1)? atoi(argv[1]):100000;
    int terms = (argc >2)? atoi(argv[2]):1000;
    int threads = (argc > 3)? atoi(argv[3]): 4;

    double pi = 3.1415926535897932384626433832795;
    double step_size = (pi)/(steps -1);

    auto start = std::chrono::high_resolution_clock::now();
    float omp_sum = 0;
    omp_set_num_threads(threads);
    #pragma omp parallel for reduction(+:omp_sum)
    for (int i = 0; i < steps; i++){
        float x = i*step_size;
        omp_sum += cos_sum(x, terms);}
    
    double omp_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count();
//Trapezoidal Rule correction
omp_sum -= (cos_sum(0, terms) + cos_sum(pi, terms)) / 2.0f;
omp_sum *= step_size;

    printf("OpenMP cosine Sum: %.10f\n", omp_sum);
    printf("OpenMP elapsed time: %.10f seconds\n", omp_time);

    return 0;
}