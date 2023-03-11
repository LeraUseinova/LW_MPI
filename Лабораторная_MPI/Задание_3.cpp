#include <iostream>
#include <cstring>
#include "mpi.h"

using namespace std;


// Data transfer around the ring. Relay baton
void ring_relay() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int message = rank;
    int recv_message;

    if (rank == 0) {
        // process 0 sends a message with its number to the next process
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        cout << "Process " << rank << " sent message " << message << endl;
        MPI_Recv(&recv_message, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process " << rank << " received message " << recv_message << endl;
    }
    else {
        // processes 1..size-1 receive a message from the previous process, increment it and send it to the next process
        MPI_Recv(&recv_message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process " << rank << " received message " << recv_message << endl;
        message = recv_message + 1;
        if (rank == size - 1) {
            // the last process sends a message to process 0
            MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            cout << "Process " << rank << " sent message " << message << endl;
        }
        else {
            // other processes send a message to the next process
            MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            cout << "Process " << rank << " sent message " << message << endl;
        }
    }
}

// Data transfer around the ring. Shift
void ring_shift() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int message = rank;
    int recv_message;

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    MPI_Request reqs[2];
    MPI_Status stats[2];

    // Sending a message to the next process
    MPI_Isend(&message, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &reqs[1]);

    // Receiving a message from the previous process
    MPI_Irecv(&recv_message, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &reqs[0]);

    // Waiting for the message to be received
    MPI_Wait(&reqs[0], &stats[0]);

    // Sending a message to the next process
    MPI_Isend(&message, 1, MPI_INT, next, 0, MPI_COMM_WORLD, &reqs[1]);

    // Waiting until all processes receive messages
    MPI_Wait(&reqs[1], &stats[1]);


    // Displaying messages
    for (int i = 0; i < size; i++) {
        if (rank == i) {
            cout << "Process " << rank << " sent message " << message << " to " << next << endl;
            cout << "Process " << rank << " received message " << recv_message << " from " << prev << endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
}



// Master-slave
void master_slave() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int message = rank;

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Recv(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process " << rank << " received message " << message << std::endl;
        }
    }
    else {
        MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

}

// All to all
void all_to_all() {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int* send_buf = new int[size]; 
    int* recv_buf = new int[size];

    // Filling the send buffer with data
    for (int i = 0; i < size; i++) {
        send_buf[i] = rank;
    }

    // Sending messages to all processes
    for (int i = 0; i < size; i++) {
        if (i == rank) {
            continue;
        }
        MPI_Send(&send_buf[0], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    // Receiving messages from all processes
    for (int i = 0; i < size; i++) {
        if (i == rank) {
            continue;
        }
        MPI_Recv(&recv_buf[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Process " << rank << " received message " << recv_buf[i] << " from process " << i << endl;
        
    }

    delete[] send_buf;
    delete[] recv_buf;
}

