#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "Process.h"

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
    vector<string> pids = convertProcesses(n);

    //run loop for each process to assign arrival, burst, etc.
    for (int i = 0; i < n; i++) {
        bool cpuBound = true;
        if (i > nCPU) {
            cpuBound = false;
        }
        string id = pids[i];
        int arrival = floor(next_exp(lambda, upperBound));
        int bursts = ceil(drand48() * 32);
        Process p(id, arrival, bursts, cpuBound);
        cout << p.getId() << " " << p.getArrivalTime() << " " << p.getNumBursts() << " " << p.isCpuBound() << endl;
        //generate each burst time
        int cpuTime, ioTime;
        for (int j = 0; j < bursts-1; j++) {
            cpuTime = ceil(next_exp(lambda, upperBound));
            ioTime = ceil(next_exp(lambda, upperBound));
            if (cpuBound) {
                cpuTime *= 4;
            } else {
                ioTime *= 8;
            }
            p.setCpuBurstTime(cpuTime);
            p.setIOBurstTime(ioTime);
        }
        //add one more cpu burst
        cpuTime = ceil(next_exp(lambda, upperBound));
        if (cpuBound) {
            cpuTime *= 4;
        }
        p.setCpuBurstTime(cpuTime);
    }
}
