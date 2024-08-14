#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <climits>
#include <iostream>

/* Generate the next random number using drand48() */
double next_exp(double lambda, int upperBound) {
    double x = INT_MAX;
    while (x > upperBound) {
        double r = drand48();
        x = -log(r) / lambda;
    }
    return x;
}