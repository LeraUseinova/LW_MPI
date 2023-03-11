#include <mpi.h>
#include <iostream>

using namespace std;

void task_four() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int tag = 0;
    int max_length = 100000;
    int count = 0;

    MPI_Request req_send, req_recv;
    MPI_Status status;

    for (int i = 1; i <= max_length; i *= 2) {
        int* buf = new int[i];
        MPI_Isend(buf, i, MPI_INT, next, tag, MPI_COMM_WORLD, &req_send);
        MPI_Irecv(buf, i, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &req_recv);

        MPI_Wait(&req_send, &status);
        MPI_Wait(&req_recv, &status);

        count++;

        delete[] buf;
        if (rank == 0) {
            cout << "Iteration " << count << ": Sent and received message of length " << i << endl;
        }
    }

    if (rank == 0) {
        cout << "Maximum length of message: " << count << endl;
    }
}