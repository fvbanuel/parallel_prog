#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
// all to all communication 
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, numprocs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    // Each process will send 4 integers to EVERY other process
    int send_count = 4; 
    
    
    int* send_buf = (int*)malloc(send_count * numprocs * sizeof(int));
    int* recv_buf = (int*)malloc(send_count * numprocs * sizeof(int));

    // Fill the send buffer data
    for (int i = 0; i < numprocs; i++) {
        send_buf[i * send_count + 0] = rank * 10 + i;
        send_buf[i * send_count + 1] = rank * 100 + i;
        send_buf[i * send_count + 2] = rank * 1000 + i;
        send_buf[i * send_count + 3] = rank * 10000 + i;
    }

    
    MPI_Alltoall(send_buf, send_count, MPI_INT, 
                 recv_buf, send_count, MPI_INT, 
                 MPI_COMM_WORLD);

    
    free(send_buf);
    free(recv_buf);

    MPI_Finalize();
    return 0;
}
