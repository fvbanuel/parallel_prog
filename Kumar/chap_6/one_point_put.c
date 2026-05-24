#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
// send a vector of size 5 from rank 0 to rank 1
// manual C-style 
int main(int argc, char** argv) {
    
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            fprintf(stderr, "This program requires at least 2 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }

    const int VECTOR_SIZE = 5;
    
    
    int* win_buffer = (int*)malloc(VECTOR_SIZE * sizeof(int));
    for (int i = 0; i < VECTOR_SIZE; i++) {
        win_buffer[i] = 0;
    }

    
    MPI_Win win;
    MPI_Win_create(
        win_buffer,                        
        VECTOR_SIZE * sizeof(int),           
        sizeof(int),                         
        MPI_INFO_NULL,                       
        MPI_COMM_WORLD,                      
        &win                                 
    );

    
    MPI_Win_fence(0, win);

    if (rank == 0) {
        // Source data to be sent
        int send_vector[5] = {10, 20, 30, 40, 50};


        MPI_Put(
            send_vector,                   
            VECTOR_SIZE,                     
            MPI_INT,                         
            1,                               
            0,                               
            VECTOR_SIZE,                     
            MPI_INT,                         
            win                              
        );
        printf("Rank 0 put vector data into Rank 1's window.\n");
    }

    
    MPI_Win_fence(0, win);

    
    if (rank == 1) {
        printf("Rank 1 window contents: ");
        for (int i = 0; i < VECTOR_SIZE; i++) {
            printf("%d ", win_buffer[i]);
        }
        printf("\n");
    }


    MPI_Win_free(&win);
    free(win_buffer);
    
    MPI_Finalize();
    return 0;
}
