#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;

// Setting the integrand.
//After change of variables the area of integration is a unit cube. Transform Jacobian = 10.
//x1 = 2 * u1 - 1;
//x2 = u2 + 1;
//x3 = 5 * u3 - 2;
double f(double u1, double u2, double u3) {
    return 10 * exp(-(u1 * u1 + u2 * u2 + u3 * u3));
}

void monte_carlo_seq() {
    auto start = chrono::high_resolution_clock::now();
    double m = 0.00001;

    //The number of points that will be generated
    int N = 2100000000;

    // Random number generator
    mt19937 mt(random_device{}());

    // Initial values for the sum and quadratic sum of the function
    double sum = 0.0, sum2 = 0.0;

    // Generating N points and calculate the function values in them
    for (int i = 0; i < N; i++) {
        double x1 = mt() / (double)mt.max();
        double x2 = mt() / (double)mt.max();
        double x3 = mt() / (double)mt.max();
        double fx = f(2 * x1 - 1, x2 + 1, 5 * x3 - 2);

        sum += fx;

        sum2 += fx * fx;
    }

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
    cout << "Time for sequential monte-carlo: " << elapsed.count() << " s" << endl;

}