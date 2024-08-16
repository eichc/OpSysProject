#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
#include <cstdio>
#include <limits.h>
#include "Process.h"

using namespace std;

string printQueue(queue<Process> q) {
    string result = "[Q";
    if (q.empty()) {
        result.append(" empty");
    } else {
        queue<Process> temp = q;
        while (!temp.empty()) {
            result.append(" ").append(temp.front().getId());
            temp.pop();
        }
    }
    result.append("]");
    return result;
}

int fcfs(vector<Process> allP, int switchTime, int nCPU, int nIO) {
    queue<Process> q;
    int time = 0;
    cout << "time " << time << "ms: Simulator started for FCFS " << printQueue(q) << endl;

    //simout variables
    double cpuBoundWaitTime = 0, ioBoundWaitTime = 0, cpuBoundBurstTime = 0, ioBoundBurstTime = 0;
    int cpuSwitches = 0, ioSwitches = 0, numCpuBoundBursts = 0, numIoBoundBursts = 0;

    int current = -1;
    unsigned int i = 0;
    int cpuCompleteTime, newArrivalTime, blockingTime, cpuStartTime, nextStartTime = -1;
    bool isCpuComplete, canStartCpu, isIOComplete, isNewArrival;
    while (!allP.empty()) {
        blockingTime = cpuStartTime = newArrivalTime = INT_MAX;
        isCpuComplete = canStartCpu = isIOComplete = isNewArrival = false;
        //cpu burst completion
        if (current != -1) {
            isCpuComplete = true;
        } else if (!q.empty()) { //start using cpu
            canStartCpu = true;
            cpuStartTime = nextStartTime;
        }
        //IO burst completion
        for (unsigned int j = 0; j < allP.size(); j++) {
            if (allP[j].getBlocking() != -1) {
                blockingTime = min(blockingTime, allP[j].getBlocking());
                isIOComplete = true;
            }
        }
        //new process arrives
        if (i < allP.size()) {
            newArrivalTime = allP[i].getArrivalTime();
            isNewArrival = true;
        }

        if (!isCpuComplete && !canStartCpu && !isIOComplete && !isNewArrival) {
            cerr << "ERROR: Nothing is able to be done\n";
            return -1;
        }

        //complete the cpu burst
        if (isCpuComplete && min(cpuCompleteTime, blockingTime) == min(cpuCompleteTime, newArrivalTime)) {
            time = cpuCompleteTime;
            if (allP[current].isCpuBound()) {
                cpuBoundBurstTime += allP[current].popFrontCPU();
                numCpuBoundBursts++;
            } else {
                ioBoundBurstTime += allP[current].popFrontCPU();
                numIoBoundBursts++;
            }
            if (allP[current].isEmptyCPU()) {
                cout << "time " << time << "ms: Process " << allP[current].getId() << " terminated " << printQueue(q) << endl;
                auto itr = allP.begin();
                while (itr->getId().compare(allP[current].getId()) != 0) {
                    itr++;
                }
                allP.erase(itr);
            } else {
                if (allP[current].getNumCPU() == 1) {
                    if (time < 10000) cout << "time " << time << "ms: Process " << allP[current].getId() << " completed a CPU burst; " << allP[current].getNumCPU() << " burst to go " << printQueue(q) << endl;
                } else {
                    if (time < 10000) cout << "time " << time << "ms: Process " << allP[current].getId() << " completed a CPU burst; " << allP[current].getNumCPU() << " bursts to go " << printQueue(q) << endl;
                }
                allP[current].setBlocking(time + allP[current].popFrontIO() + switchTime/2);
                if (time < 10000) cout << "time " << time << "ms: Process " << allP[current].getId() << " switching out of CPU; blocking on I/O until time " << allP[current].getBlocking() << "ms " << printQueue(q) << endl;
            }
            time += switchTime/2;
            current = -1;
            cpuCompleteTime = INT_MAX;
            if (!q.empty()) {
                nextStartTime = time + switchTime/2;
            } else {
                nextStartTime = -1;
            }
            
        } else if (canStartCpu && min(cpuStartTime, blockingTime) == min(cpuStartTime, newArrivalTime)) { //start next cpu burst
            for (unsigned int j = 0; j < allP.size(); j++) {
                if (allP[j].getId().compare(q.front().getId()) == 0) {
                    current = j;
                    break;
                }
            }
            q.pop();
            if (nextStartTime == -1) {
                time += switchTime/2;
            } else {
                time = nextStartTime;
            }
            if (allP[current].isCpuBound()) {
                cpuBoundWaitTime += (time - allP[current].getWaiting() - switchTime/2);
                cpuSwitches++;
            } else {
                ioBoundWaitTime += (time - allP[current].getWaiting() - switchTime/2);
                ioSwitches++;
            }
            int temp = allP[current].getFrontCPU();
            if (time < 10000) cout << "time " << time << "ms: Process " << allP[current].getId() << " started using the CPU for " << temp << "ms burst " << printQueue(q) << endl;
            cpuCompleteTime = time + temp;
        } else if (blockingTime < newArrivalTime) { //complete the IO burst
            time = blockingTime;
            for (unsigned int j = 0; j < allP.size(); j++) {
                if (allP[j].getBlocking() == time) {
                    allP[j].setBlocking(-1);
                    q.push(allP[j]);
                    if (time < 10000) cout << "time " << time << "ms: Process " << allP[j].getId() << " completed I/O; added to ready queue " << printQueue(q) << endl;
                    allP[j].setWaiting(time);
                    break;
                }
            }
            
        } else { //new process arrives
            q.push(allP[i]);
            time = allP[i].getArrivalTime();
            if (time < 10000) cout << "time " << time << "ms: Process " << allP[i].getId() << " arrived; added to ready queue " << printQueue(q) << endl;
            allP[i].setWaiting(time);
            i++;
        }
    }
    
    cout << "time " << time << "ms: Simulator ended for FCFS " << printQueue(q) << endl;

    //simout
    FILE *fp;
    fp = fopen("simout.txt", "a");

    double utilization = 0;
    if (time != 0) {
        utilization = ceil(((cpuBoundBurstTime + ioBoundBurstTime)/time) * 100000) / 1000;
    }
    double cpuBoundAvgWait = 0;
    if (numCpuBoundBursts != 0) {
        cpuBoundAvgWait = ceil((cpuBoundWaitTime/numCpuBoundBursts) * 1000) / 1000;
    }
    double ioBoundAvgWait = 0;
    if (numIoBoundBursts != 0) {
        ioBoundAvgWait = ceil((ioBoundWaitTime/numIoBoundBursts) * 1000) / 1000;
    }
    double totalAvgWait = 0;
    if (numCpuBoundBursts + numIoBoundBursts != 0) {
        totalAvgWait = ceil(((cpuBoundWaitTime + ioBoundWaitTime)/(numCpuBoundBursts + numIoBoundBursts)) * 1000) / 1000;
    }

    fprintf(fp, "\nAlgorithm FCFS\n");
    fprintf(fp, "-- CPU utilization: %.3f%%\n", utilization);
    fprintf(fp, "-- CPU-bound average wait time: %.3f ms\n", cpuBoundAvgWait);
    fprintf(fp, "-- I/O-bound average wait time: %.3f ms\n", ioBoundAvgWait);
    fprintf(fp, "-- overall average wait time: %.3f ms\n", totalAvgWait);
    // fprintf(fp, "-- CPU-bound average turnaround time: %.3f ms\n");
    // fprintf(fp, "-- I/O-bound average turnaround time: %.3f ms\n");
    // fprintf(fp, "-- overall average turnaround time: %.3f ms\n");
    fprintf(fp, "-- CPU-bound number of context switches: %d\n", cpuSwitches);
    fprintf(fp, "-- I/O-bound number of context switches: %d\n", ioSwitches);
    fprintf(fp, "-- overall number of context switches: %d\n", cpuSwitches + ioSwitches);
    fprintf(fp, "-- CPU-bound number of preemptions: 0\n");
    fprintf(fp, "-- I/O-bound number of preemptions: 0\n");
    fprintf(fp, "-- overall number of preemptions: 0\n");

    fclose(fp);
    return 0;
}