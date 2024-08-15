#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>
#include "Process.h"

using namespace std;

extern vector<string> convertProcesses(int p);
extern double next_exp(double lambda, int upperBound);
extern void printSimout(int p, int CPUp, int IOp, double CPUavgCPUBurst, double IOavgCPUBurst, double avgCPUBurst, 
    double CPUavgIOBurst, double IOavgIOBurst, double avgIOBurst);
extern int fcfs(vector<Process> allP, int switchTime);

extern void ShortestJobFirst(std::vector<Process> processes,int switchTime, double lambda, double alpha);


bool checkArgs(char* argv[], int n, int nCPU, unsigned int seed, double lambda, int upperBound, int switchTime, double alpha, int slice) {
    if (string(argv[1]).compare(to_string(n)) != 0 || n < 1) {
        fprintf(stderr, "ERROR: argv[1] must be an int > 0\n");
        return false;
    }
    if (string(argv[2]).compare(to_string(nCPU)) != 0 || nCPU < 0) {
        fprintf(stderr, "ERROR: argv[2] must be a positive int\n");
        return false;
    }
    if (string(argv[3]).compare(to_string(seed)) != 0 || seed < 0) {
        fprintf(stderr, "ERROR: argv[3] must be a positive int\n");
        return false;
    }
    if (lambda < 0) {
        fprintf(stderr, "ERROR: argv[4] must be a positive double\n");
        return false;
    }
    if (string(argv[5]).compare(to_string(upperBound)) != 0 || upperBound < 0) {
        fprintf(stderr, "ERROR: argv[5] must be a positive int\n");
        return false;
    }
    if (string(argv[6]).compare(to_string(switchTime)) || switchTime < 0 || switchTime % 2 != 0) {
        fprintf(stderr, "ERROR: argv[6] must be an even, positive int\n");
        return false;
    }
    if (alpha < 0 || alpha > 1) {
        fprintf(stderr, "ERROR: argv[7] must be in the range [0, 1]\n");
        return false;
    }
    if (string(argv[8]).compare(to_string(slice)) != 0 || slice < 0) {
        fprintf(stderr, "ERROR: argv[8] must be a positive int\n");
        return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    //error check
    if (argc != 9) {
        fprintf(stderr, "ERROR: Incorrect number of arguments\n");
        return -1;
    }

    //get command line args
    int n = atoi(*(argv+1));
    int nCPU = atoi(*(argv+2));
    unsigned int seed = atoi(*(argv+3));
    double lambda = atof(*(argv+4));
    int upperBound = atoi(*(argv+5));
    int switchTime = atoi(*(argv+6));
    double alpha = atof(*(argv+7));
    int slice = atoi(*(argv+8));
    int nIO = n-nCPU;
    double CPUavgCPUBurst = 0, IOavgCPUBurst = 0, avgCPUBurst = 0, CPUavgIOBurst = 0, IOavgIOBurst = 0, avgIOBurst = 0;
    int numCPUavgCPUBurst=0, numIOavgCPUBurst=0, numCPUavgIOBurst=0, numIOavgIOBurst=0;

    //more error checking
    if (!checkArgs(argv, n, nCPU, seed, lambda, upperBound, switchTime, alpha, slice)) {
        return -1;
    }
    
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
    printf("<<< -- seed=%d; lambda=%.6f; bound=%d\n", seed, lambda, upperBound);
    vector<Process> allP;

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
        allP.push_back(p);
    }

    //calculate simout, avoiding division by 0
    avgCPUBurst = CPUavgCPUBurst+IOavgCPUBurst;
    avgIOBurst = CPUavgIOBurst+IOavgIOBurst;
    if (numCPUavgCPUBurst+numIOavgCPUBurst != 0) {
        avgCPUBurst = ceil((avgCPUBurst/(numCPUavgCPUBurst+numIOavgCPUBurst)) * 1000) / 1000;
    }
    if (numIOavgIOBurst+numCPUavgIOBurst != 0) {
        avgIOBurst = ceil((avgIOBurst/(numIOavgIOBurst+numCPUavgIOBurst)) * 1000) / 1000;
    }
    if (numCPUavgCPUBurst != 0) {
        CPUavgCPUBurst = ceil((CPUavgCPUBurst/numCPUavgCPUBurst) * 1000) / 1000;
    }
    if (numCPUavgIOBurst != 0) {
        CPUavgIOBurst = ceil((CPUavgIOBurst/numCPUavgIOBurst) * 1000) / 1000;
    }
    if (numIOavgCPUBurst != 0) {
        IOavgCPUBurst = ceil((IOavgCPUBurst/numIOavgCPUBurst) * 1000) / 1000;
    }
    if (numIOavgIOBurst != 0) {
        IOavgIOBurst = ceil((IOavgIOBurst/numIOavgIOBurst) * 1000) / 1000;
    }
    printSimout(n, nCPU, nIO, CPUavgCPUBurst, IOavgCPUBurst, avgCPUBurst, CPUavgIOBurst, IOavgIOBurst, avgIOBurst);


    /**
     * 
     * 
     * PART 2 STARTS HERE
     * 
     */

    printf("\n<<< PROJECT PART II\n");
    // printf("<<< -- t_cs=%dms; alpha=%f; t_slice=%dms\n", switchTime, alpha, slice);
    // if (fcfs(allP, switchTime) == -1) {
    //     return -1;
    // }


    //testing the sjf thing
    ShortestJobFirst(allP,switchTime, lambda, alpha);


}
