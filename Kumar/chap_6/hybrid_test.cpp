#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
// hybrid program for communication of vector between 2 nodes using full thread safety
int main(int argc, char* argv[]) {
    
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // 2 MPI processes/nodes
    if (size != 2) {
        if (rank == 0) {
            std::cerr << "Error: This specific test must be run with exactly 2 MPI processes (-n 2)." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    //  OpenMP Thread Check 
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        
        
        #pragma omp critical
        {
            std::cout << "[MPI Rank " << rank << "] OpenMP Thread " 
                      << thread_id << "/" << total_threads 
                      << " initialized successfully." << std::endl;
        }
    }

    
    MPI_Barrier(MPI_COMM_WORLD);

    // data exchange
    const int SIZE = 1000;
    std::vector<int> msg_buffer(SIZE, rank); // Filled with 0s for Rank 0, 1s for Rank 1

    if (rank == 0) {
        std::cout << "\n[MPI Rank 0] Sending data packet to Rank 1..." << std::endl;
        MPI_Send(msg_buffer.data(), SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD);
        
        // Wait to get data back
        MPI_Recv(msg_buffer.data(), SIZE, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "[MPI Rank 0] Successfully received Rank 1's validation data (Value: " 
                  << msg_buffer[0] << ")." << std::endl;
    } 
    else if (rank == 1) {
        std::vector<int> recv_buffer(SIZE);
        MPI_Recv(recv_buffer.data(), SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "[MPI Rank 1] Successfully intercepted Rank 0's validation data (Value: " 
                  << recv_buffer[0] << ")." << std::endl;
        
        std::cout << "[MPI Rank 1] Sending confirmation back to Rank 0..." << std::endl;
        MPI_Send(msg_buffer.data(), SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
