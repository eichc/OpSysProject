#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cmath>
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

int fcfs(vector<Process> allP, int switchTime) {
    queue<Process> q;
    int time = 0;
    cout << "time " << time << "ms: Simulator started for FCFS " << printQueue(q) << endl;

    //simout variables
    double cpuBoundWaitTime = 0, ioBoundWaitTime = 0, cpuBoundBurstTime = 0, ioBoundBurstTime = 0;
    int cpuSwitches = 0, ioSwitches = 0, numCpuBoundBursts = 0, numIoBoundBursts = 0;

    int current = -1;
    unsigned int i = 0;
    int cpuCompleteTime, newArrivalTime, blockingTime, cpuStartTime, switchingIn = -1, cpuSwitchTime = 0;
    int switchingOut = -1, switchOutTime = 0;
    bool isCpuComplete, canStartCpu, isIOComplete, isNewArrival, canSwitchIn, canSwitchOut;
    while (!allP.empty()) {
        blockingTime = newArrivalTime = INT_MAX;
        isCpuComplete = canStartCpu = isIOComplete = isNewArrival = canSwitchIn = canSwitchOut = false;
        // CPU burst completion
        if (current != -1) {
            if (switchingOut != -1) {
                canSwitchOut = true;
            } else {
                isCpuComplete = true;
            }
        } else if (switchingIn != -1) { // Start using CPU
            canStartCpu = true;
            cpuStartTime = cpuSwitchTime + switchTime/2;
        } else if (!q.empty()) { // Switch into CPU
            canSwitchIn = true;
            cpuSwitchTime = max(time, switchOutTime);
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

        if (!isCpuComplete && !canStartCpu && !isIOComplete && !isNewArrival && !canSwitchIn && !canSwitchOut) {
            cerr << "ERROR: Nothing is able to be done\n";
            return -1;
        }

        //complete the cpu burst
        if (isCpuComplete && cpuCompleteTime <= blockingTime && cpuCompleteTime <= newArrivalTime) {
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
                i--;
            } else {
                if (allP[current].getNumCPU() == 1) {
                    #ifndef DEBUG_MODE_FCFS
                    if (time < 10000) {
                    #endif
                        cout << "time " << time << "ms: Process " << allP[current].getId() << " completed a CPU burst; " << allP[current].getNumCPU() << " burst to go " << printQueue(q) << endl;
                    #ifndef DEBUG_MODE_FCFS
                    }
                    #endif
                } else {
                    #ifndef DEBUG_MODE_FCFS
                    if (time < 10000) {
                    #endif
                        cout << "time " << time << "ms: Process " << allP[current].getId() << " completed a CPU burst; " << allP[current].getNumCPU() << " bursts to go " << printQueue(q) << endl;
                    #ifndef DEBUG_MODE_FCFS
                    }
                    #endif
                }
                allP[current].setBlocking(time + allP[current].popFrontIO() + switchTime/2);
                #ifndef DEBUG_MODE_FCFS
                if (time < 10000) {
                #endif
                    cout << "time " << time << "ms: Process " << allP[current].getId() << " switching out of CPU; blocking on I/O until time " << allP[current].getBlocking() << "ms " << printQueue(q) << endl;
                #ifndef DEBUG_MODE_FCFS
                }
                #endif
            }
            switchOutTime = time + switchTime/2;
            switchingOut = current;
            
        } else if (canSwitchOut && switchOutTime <= blockingTime && switchOutTime <= newArrivalTime) { //switch out of cpu
            time = switchOutTime;
            current = -1;
            switchingOut = -1;
            cpuCompleteTime = INT_MAX;

        } else if (canSwitchIn && cpuSwitchTime < blockingTime && cpuSwitchTime < newArrivalTime) { //switch into cpu
            time = cpuSwitchTime;
            for (unsigned int j = 0; j < allP.size(); j++) {
                if (allP[j].getId().compare(q.front().getId()) == 0) {
                    switchingIn = j;
                    break;
                }
            }
            q.pop();
            if (allP[switchingIn].isCpuBound()) {
                cpuBoundWaitTime += (time - allP[switchingIn].getWaiting());
                cpuSwitches++;
            } else {
                ioBoundWaitTime += (time - allP[switchingIn].getWaiting());
                ioSwitches++;
            }

        } else if (canStartCpu && cpuStartTime <= blockingTime && cpuStartTime <= newArrivalTime) { //start next cpu burst
            time = cpuStartTime;
            current = switchingIn;
            switchingIn = -1;

            int temp = allP[current].getFrontCPU();
            #ifndef DEBUG_MODE_FCFS
            if (time < 10000) {
            #endif
                cout << "time " << time << "ms: Process " << allP[current].getId() << " started using the CPU for " << temp << "ms burst " << printQueue(q) << endl;
            #ifndef DEBUG_MODE_FCFS
            }
            #endif
            cpuCompleteTime = time + temp;

        } else if (blockingTime <= newArrivalTime) { //complete the IO burst
            time = blockingTime;
            unsigned int j;
            int selector = -1;
            for (j = 0; j < allP.size(); j++) {
                if (allP[j].getBlocking() == time) {
                    if (selector == -1 || allP[j].getId().compare(allP[selector].getId()) < 0) {
                        selector = j;
                    }
                }
            }
            allP[selector].setBlocking(-1);
            q.push(allP[selector]);
            #ifndef DEBUG_MODE_FCFS
            if (time < 10000) {
            #endif
                std::cout << "time " << time << "ms: Process " << allP[selector].getId() << " completed I/O; added to ready queue " << printQueue(q) << std::endl;
            #ifndef DEBUG_MODE_FCFS
            }
            #endif
            allP[selector].setWaiting(time);
            
        } else { //new process arrives
            q.push(allP[i]);
            time = allP[i].getArrivalTime();
            #ifndef DEBUG_MODE_FCFS
            if (time < 10000) {
            #endif
                cout << "time " << time << "ms: Process " << allP[i].getId() << " arrived; added to ready queue " << printQueue(q) << endl;
            #ifndef DEBUG_MODE_FCFS
            }
            #endif
            allP[i].setWaiting(time);
            i++;
        }
    }
    
    time += switchTime/2;
    cout << "time " << time << "ms: Simulator ended for FCFS " << printQueue(q) << endl;

    //simout
    FILE *fp;
    fp = fopen("simout.txt", "a");

    double utilization = 0;
    if (time != 0) {
        utilization = ceil(((cpuBoundBurstTime + ioBoundBurstTime)/time) * 100000) / 1000;
    }
    double cpuBoundAvgWait = 0, cpuBoundAvgTurn = 0;;
    if (numCpuBoundBursts != 0) {
        cpuBoundAvgWait = ceil((cpuBoundWaitTime/numCpuBoundBursts) * 1000) / 1000;
        cpuBoundAvgTurn = ceil(( (cpuBoundWaitTime+cpuBoundBurstTime+(cpuSwitches*switchTime)) / numCpuBoundBursts) * 1000) / 1000;
    }
    double ioBoundAvgWait = 0, ioBoundAvgTurn = 0;
    if (numIoBoundBursts != 0) {
        ioBoundAvgWait = ceil((ioBoundWaitTime/numIoBoundBursts) * 1000) / 1000;
        ioBoundAvgTurn = ceil(( (ioBoundWaitTime+ioBoundBurstTime+(ioSwitches*switchTime)) / numIoBoundBursts) * 1000) / 1000;
    }
    double totalAvgWait = 0, totalAvgTurn = 0;
    if (numCpuBoundBursts + numIoBoundBursts != 0) {
        totalAvgWait = ceil(((cpuBoundWaitTime + ioBoundWaitTime)/(numCpuBoundBursts + numIoBoundBursts)) * 1000) / 1000;
        totalAvgTurn = ceil((((cpuBoundWaitTime+cpuBoundBurstTime+(cpuSwitches*switchTime)) + (ioBoundWaitTime+ioBoundBurstTime+(ioSwitches*switchTime)))/(numCpuBoundBursts + numIoBoundBursts)) * 1000) / 1000;
    }

    fprintf(fp, "\nAlgorithm FCFS\n");
    fprintf(fp, "-- CPU utilization: %.3f%%\n", utilization);
    fprintf(fp, "-- CPU-bound average wait time: %.3f ms\n", cpuBoundAvgWait);
    fprintf(fp, "-- I/O-bound average wait time: %.3f ms\n", ioBoundAvgWait);
    fprintf(fp, "-- overall average wait time: %.3f ms\n", totalAvgWait);
    fprintf(fp, "-- CPU-bound average turnaround time: %.3f ms\n", cpuBoundAvgTurn);
    fprintf(fp, "-- I/O-bound average turnaround time: %.3f ms\n", ioBoundAvgTurn);
    fprintf(fp, "-- overall average turnaround time: %.3f ms\n", totalAvgTurn);
    fprintf(fp, "-- CPU-bound number of context switches: %d\n", cpuSwitches);
    fprintf(fp, "-- I/O-bound number of context switches: %d\n", ioSwitches);
    fprintf(fp, "-- overall number of context switches: %d\n", cpuSwitches + ioSwitches);
    fprintf(fp, "-- CPU-bound number of preemptions: 0\n");
    fprintf(fp, "-- I/O-bound number of preemptions: 0\n");
    fprintf(fp, "-- overall number of preemptions: 0\n");

    fclose(fp);
    return 0;
}