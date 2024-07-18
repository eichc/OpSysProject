#include <string>
#include <iostream>

int main(int argc, char *argv[]) {
    //get command line args
    int n = atoi(*(argv+1));
    int nCPU = atoi(*(argv+2));
    unsigned int seed = atoi(*(argv+3));
    double lambda = atoi(*(argv+4));
    int upperBound = atoi(*(argv+5));
    
    //initialize random number generator
    srand48(seed);
}
