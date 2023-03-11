#include <iostream>
#include <cstring>
#include "mpi.h"

using namespace std;

void task_two() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // getting the rank of the current process
    double start_time, end_time; 
    char message1[] = "Hello from process 0";
    char message2[] = "Hello you process 0, it is process 1";
    if (rank == 0) {
        // a process with rank 0 sends a message to a process with rank 1
        start_time = MPI_Wtime();
        MPI_Send(message1, strlen(message1) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(message2, strlen(message2) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end_time = MPI_Wtime();
        cout << "Process 0 received message " << message2 << endl;
    }
    else if (rank == 1) {
        // a process with rank 1 receives a message from a process with rank 0
        MPI_Recv(message1, strlen(message1) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(message2, strlen(message2) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        cout << "Process 1 received message " << message1 << endl;
    }
    if (rank == 0) {
        // time output per message exchange iteration
        cout << "Time per exchange: " << (end_time - start_time) << " seconds" << endl;
    }

    
}


