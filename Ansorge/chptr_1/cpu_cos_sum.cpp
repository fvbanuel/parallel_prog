#include <stdio.h>
#include <stdlib.h>
#include <chrono>
// cosine integral sum
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

    double pi = 3.1415926535897932384626433832795;
    double step_size = (pi)/(steps -1);

    auto start = std::chrono::high_resolution_clock::now();
    float cpu_sum = 0;
    for (int i = 0; i < steps; i++){
        float x = i*step_size;
        cpu_sum += cos_sum(x, terms);}
    
    double cpu_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count();
//Trapezoidal Rule correction
cpu_sum -= (cos_sum(0, terms) + cos_sum(pi, terms)) / 2.0f;
cpu_sum *= step_size;

    printf("CPU cosine Sum: %.10f\n", cpu_sum);
    printf("CPU elapsed time: %.10f seconds\n", cpu_time);

    return 0;

    }
