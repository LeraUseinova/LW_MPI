#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <mpi.h>

using namespace std;

// Setting the integrand.
//After change of variables the area of integration is a unit cube. Transform Jacobian = 10.
//x1 = 2 * u1 - 1;
//x2 = u2 + 1;
//x3 = 5 * u3 - 2;
double f_mpi(double u1, double u2, double u3) {
    return 10 * exp(-(u1 * u1 + u2 * u2 + u3 * u3));
}

void monte_carlo_mpi() {
    auto start = chrono::high_resolution_clock::now();

    double m = 0.00001;
    int N = 2100000000;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // The number of points that each process will generate
    int N_proc = N / size;

    // Random number generator
    mt19937 mt(random_device{}());

    // Initial values for the sum and quadratic sum of the function for each process
    double sum_proc = 0.0, sum2_proc = 0.0;

    // Generating N points and calculate the function values in them
    for (int i = 0; i < N_proc; i++) {
        double x1 = mt() / (double)mt.max();
        double x2 = mt() / (double)mt.max();
        double x3 = mt() / (double)mt.max();
        double fx = f_mpi(2 * x1 - 1, x2 + 1, 5 * x3 - 2);

        sum_proc += fx;

        sum2_proc += fx * fx;
    }

    // Combining the results obtained on each process
    double sum = 0.0, sum2= 0.0;
    MPI_Reduce(&sum_proc, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sum2_proc, &sum2, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Sending the sum to process 0, which will perform the calculation and display the result
    if (rank == 0) {
        // Calculating the mean value of the function and its standard deviation
        double mean = sum / N;
        double std_dev = sqrt((sum2 / N) - (mean * mean));

        // Calculating the value of the integral and estimate its error
        double I = mean;
        double error = std_dev / sqrt(N);

        cout << "Integral value: " << I << endl;
        cout << "Error estimate: " << error << endl;

        // Checking if the required accuracy is achieved
        if (error <= m) {
            cout << "Desired accuracy reached." << endl;
        }
        else {
            cout << "Desired accuracy not reached." << endl;
        }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end - start;
        cout << "Time for mpi monte-carlo: " << elapsed.count() << " s" << endl;
    }
    
}