#include <mpi.h>
#include <cstdlib>
#include <iostream>
//sending last two columns of a 4x5 matrix
// adapted from listing 6.18
int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank,size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Datatype fColumn2x5;
    int matrix[4][5];

    MPI_Type_vector(4, 2, 5, MPI_INT, &fColumn2x5);
    MPI_Type_commit(&fColumn2x5);

    if (rank == 0) {
        // Start at row 0, column 3 (the beginning of the last two columns)
        MPI_Send(&matrix[0][3], 1, fColumn2x5, 1, 0, MPI_COMM_WORLD);
    } else if (rank == 1) {
        int recv_buffer[4][2]; // Space for 4 rows, 2 columns
        // The receiver can receive it into a contiguous block
        MPI_Recv(&recv_buffer[0][0], 8, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Type_free(&fColumn2x5);

    MPI_Finalize();
    return 0;
}



