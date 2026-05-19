#include <mpi.h>
#include<stdio.h>
#include<stdlib.h>

// collective communications of a 4- vector
// Using raw C arrays
// would use std::vector in C++

int main( int argc, char* argv[]){

    MPI_Init(&argc,&argv);
    int numprocs,rank;
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int vec[4] = {2,5,1,0};
    int* recv_vec = NULL;
    
    if (rank == 0){
        recv_vec = (int*) malloc(4*numprocs*sizeof(int));
    }
    
    MPI_Gather(vec,4,MPI_INT,recv_vec,4,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}