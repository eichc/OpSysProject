#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "Process.h"

using namespace std;

extern vector<string> convertProcesses(int p);
extern double next_exp(double lambda, int upperBound);
extern void printSimout(int p, int CPUp, int IOp, double CPUavgCPUBurst, double IOavgCPUBurst, double avgCPUBurst, 
    double CPUavgIOBurst, double IOavgIOBurst, double avgIOBurst);

int main(int argc, char *argv[]) {
    //get command line args
    int n = atoi(*(argv+1));
    int nCPU = atoi(*(argv+2));
    unsigned int seed = atoi(*(argv+3));
    double lambda = atof(*(argv+4));
    int upperBound = atoi(*(argv+5));
    int nIO = n-nCPU;
    double CPUavgCPUBurst, IOavgCPUBurst, avgCPUBurst, CPUavgIOBurst, IOavgIOBurst, avgIOBurst;
    int numCPUavgCPUBurst=0, numIOavgCPUBurst=0, numCPUavgIOBurst=0, numIOavgIOBurst=0;
    
    //initialize random number generator and process ids
    srand48(seed);
    vector<string> pids = convertProcesses(n);

    //general output
    printf("<<< PROJECT PART I\n");
    if (nCPU == 1) {
        printf("<<< -- process set (n=%d) with %d CPU-bound process\n", n, nCPU);
    } else {
        printf("<<< -- process set (n=%d) with %d CPU-bound processes\n", n, nCPU);
    }
    printf("<<< -- seed=%d; lambda=%lf; bound=%d\n", seed, lambda, upperBound);

    //run loop for each process to assign arrival, burst, etc.
    for (int i = 0; i < n; i++) {
        bool cpuBound = true;
        if (i >= nCPU) {
            cpuBound = false;
        }
        string id = pids[i];
        int arrival = floor(next_exp(lambda, upperBound));
        int bursts = ceil(drand48() * 32);
        Process p(id, arrival, bursts, cpuBound);

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
            if (cpuBound) {
                CPUavgCPUBurst+=cpuTime;
                CPUavgIOBurst+=ioTime;
                numCPUavgCPUBurst++;
                numCPUavgIOBurst++;
            }
            else {
                IOavgCPUBurst+=cpuTime;
                IOavgIOBurst+=ioTime;
                numIOavgCPUBurst++;
                numIOavgIOBurst++;
            }
        }
        //add one more cpu burst without I/O
        cpuTime = ceil(next_exp(lambda, upperBound));
        if (cpuBound) {
            cpuTime *= 4;

            CPUavgCPUBurst+=cpuTime;
            numCPUavgCPUBurst++;
        } else {
            IOavgCPUBurst+=cpuTime;
            numIOavgCPUBurst++;
        }
        p.setCpuBurstTime(cpuTime);

        //output
        p.outputProcess();
    }
    avgCPUBurst = CPUavgCPUBurst+IOavgCPUBurst;
    avgIOBurst = CPUavgIOBurst+IOavgIOBurst;
    avgCPUBurst = ceil((avgCPUBurst/(numCPUavgCPUBurst+numIOavgCPUBurst)) * 1000) / 1000;
    avgIOBurst = ceil((avgIOBurst/(numIOavgIOBurst+numCPUavgIOBurst)) * 1000) / 1000;
    CPUavgCPUBurst = ceil((CPUavgCPUBurst/numCPUavgCPUBurst) * 1000) / 1000;
    CPUavgIOBurst = ceil((CPUavgIOBurst/numCPUavgIOBurst) * 1000) / 1000;
    IOavgCPUBurst = ceil((IOavgCPUBurst/numIOavgCPUBurst) * 1000) / 1000;
    IOavgIOBurst = ceil((IOavgIOBurst/numIOavgIOBurst) * 1000) / 1000;
    printSimout(n, nCPU, nIO, CPUavgCPUBurst, IOavgCPUBurst, avgCPUBurst, CPUavgIOBurst, IOavgIOBurst, avgIOBurst);
}
