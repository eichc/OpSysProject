#include <string>
#include <iostream>
#include <vector>

using namespace std;

extern vector<string> convertProcesses(int p);
extern double next_exp(double lambda, int upperBound);

int main(int argc, char *argv[]) {
    //get command line args
    int n = atoi(*(argv+1));
    int nCPU = atoi(*(argv+2));
    unsigned int seed = atoi(*(argv+3));
    double lambda = atof(*(argv+4));
    int upperBound = atoi(*(argv+5));
    
    //initialize random number generator and process ids
    srand48(seed);
    vector<string> convertProcesses(n);

    //run loop for each process to assign arrival, burst, etc.
    for (int i = 0; i < n; i++) {
        
    }
}
