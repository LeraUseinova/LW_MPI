#include <iostream>
#include "mpi.h"
# include "Header.h"

int main(int argc, char* argv[]) {

	MPI_Init(&argc, &argv);

	//task_two();
	 
	// task 3
	//ring_relay();
	//ring_shift();
	//master_slave();
	//all_to_all();

	//task_four();
	
	monte_carlo_mpi();

	MPI_Finalize();

	//monte_carlo_seq();
	return 0;
}

