#include <queue>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <limits.h>
#include <fstream>  
#include "Process.h"

std::string printQueue(std::priority_queue<Process> q) {
    std::string result = "[Q";
    if (q.empty()) {
        result.append(" empty");
    } else {
        while (!q.empty()) {
            result.append(" ").append(q.top().getId());
            q.pop();
        }
    }
    result.append("]");
    return result;
}

void ShortestJobFirst(std::vector<Process> processes, int switchTime, double lambda, double alpha) {
    std::priority_queue<Process> SJFqueue;
    int currentTime = 0;  
    int current = -1; 
    int cpuCompleteTime = INT_MAX;
    unsigned int i = 0;
    int blockingTime, cpuStartTime, newArrivalTime;
    bool isCpuComplete, canStartCpu, isIOComplete, isNewArrival;

    // Variables to track statistics
    double cpuBoundWaitTime = 0, ioBoundWaitTime = 0;
    double cpuBoundTurnaroundTime = 0, ioBoundTurnaroundTime = 0;
    double cpuBoundBurstTime = 0, ioBoundBurstTime = 0;
    int cpuSwitches = 0, ioSwitches = 0;
    int numCpuBoundBursts = 0, numIoBoundBursts = 0;
    double cpuBusyTime = 0; 

    std::cout << "time 0ms: Simulator started for SJF [Q empty]" << std::endl;

    // Calculate the initial tau based on the given lambda
    int initialTau = std::ceil(1 / lambda);
    
    // Initialize tau for each process
    for (auto& process : processes) {
        process.setTau(initialTau);
    }

    while (!processes.empty()) {
        blockingTime = cpuStartTime = newArrivalTime = INT_MAX;
        isCpuComplete = canStartCpu = isIOComplete = isNewArrival = false;

        // CPU burst completion
        if (current != -1) {
            isCpuComplete = true;
        } else if (!SJFqueue.empty()) {  // Start using CPU
            canStartCpu = true;
            cpuStartTime = currentTime + switchTime / 2;
        }

        // I/O burst completion
        for (unsigned int j = 0; j < processes.size(); j++) {
            if (processes[j].getBlocking() != -1) {
                blockingTime = std::min(blockingTime, processes[j].getBlocking());
                isIOComplete = true;
            }
        }

        // New process arrives
        if (i < processes.size()) {
            newArrivalTime = processes[i].getArrivalTime();
            isNewArrival = true;
        }

        if (!isCpuComplete && !canStartCpu && !isIOComplete && !isNewArrival) {
            std::cerr << "ERROR: Nothing is able to be done\n";
            return;
        }

        // Complete the CPU burst
        if (isCpuComplete && std::min(cpuCompleteTime, blockingTime) == std::min(cpuCompleteTime, newArrivalTime)) {
            currentTime = cpuCompleteTime;
            int burstTime = processes[current].popFrontCPU();
            
            if (processes[current].isCpuBound()) {
                cpuBoundBurstTime += burstTime;
                numCpuBoundBursts++;
                cpuBusyTime += burstTime;
            } else {
                ioBoundBurstTime += burstTime;
                numIoBoundBursts++;
                cpuBusyTime += burstTime;
            }

            // Update the turnaround time
            if (processes[current].isCpuBound()) {
                cpuBoundTurnaroundTime += (cpuBoundWaitTime + cpuBoundBurstTime + (cpuSwitches * switchTime));
            } else {
                ioBoundTurnaroundTime += (ioBoundWaitTime + ioBoundBurstTime + (ioSwitches * switchTime));
            }

            if (!processes[current].isEmptyCPU()) {
                if (currentTime <= 9999) {
                    if (processes[current].getNumCPU() == 1) {
                        std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " (tau " << processes[current].getTau() << "ms) completed a CPU burst; " << processes[current].getNumCPU() << " burst to go " << printQueue(SJFqueue) << std::endl;
                    } else {
                        std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " (tau " << processes[current].getTau() << "ms) completed a CPU burst; " << processes[current].getNumCPU() << " bursts to go " << printQueue(SJFqueue) << std::endl;
                    }
                }

                int oldTau = processes[current].getTau();
                int newTau = std::ceil((alpha * burstTime) + ((1 - alpha) * oldTau));
                processes[current].setTau(newTau);

                if (currentTime <= 9999) {
                    std::cout << "time " << currentTime << "ms: Recalculated tau for process " << processes[current].getId() 
                              << ": old tau " << oldTau << "ms ==> new tau " << newTau << "ms " << printQueue(SJFqueue) << std::endl;
                }

                processes[current].setBlocking(currentTime + processes[current].popFrontIO() + switchTime / 2);
                if (currentTime <= 9999) {
                    std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " switching out of CPU; blocking on I/O until time " << processes[current].getBlocking() << "ms " << printQueue(SJFqueue) << std::endl;
                }
            } else {
                if (currentTime <= 9999) {
                    std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " terminated " << printQueue(SJFqueue) << std::endl;
                } else {
                    std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " terminated [Q empty]" << std::endl;
                }
                processes.erase(processes.begin() + current);
            }

            currentTime += switchTime / 2;
            current = -1;
            cpuCompleteTime = INT_MAX;

        } else if (canStartCpu && std::min(cpuStartTime, blockingTime) == std::min(cpuStartTime, newArrivalTime)) { // Start next CPU burst
            for (unsigned int j = 0; j < processes.size(); j++) {
                if (processes[j].getId().compare(SJFqueue.top().getId()) == 0) {
                    current = j;
                    break;
                }
            }
            SJFqueue.pop();
            currentTime += switchTime / 2;
            int temp = processes[current].getFrontCPU();
            
            if (processes[current].isCpuBound()) {
                cpuBoundWaitTime += (currentTime - processes[current].getWaiting() - switchTime / 2);
                cpuSwitches++;
            } else {
                ioBoundWaitTime += (currentTime - processes[current].getWaiting() - switchTime / 2);
                ioSwitches++;
            }
            
            if (currentTime <= 9999) {
                std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " (tau " << processes[current].getTau() << "ms) started using the CPU for " << temp << "ms burst " << printQueue(SJFqueue) << std::endl;
            }
            cpuCompleteTime = currentTime + temp;
        } else if (blockingTime < newArrivalTime) { // Complete the I/O burst
            currentTime = blockingTime;
            for (unsigned int j = 0; j < processes.size(); j++) {
                if (processes[j].getBlocking() == currentTime) {
                    processes[j].setBlocking(-1);
                    SJFqueue.push(processes[j]);
                    if (currentTime <= 9999) {
                        std::cout << "time " << currentTime << "ms: Process " << processes[j].getId() <<" (tau " << processes[j].getTau() << "ms) completed I/O; added to ready queue " << printQueue(SJFqueue) << std::endl;
                    }
                    processes[j].setWaiting(currentTime);
                    break;
                }
            }
            
        } else { // New process arrives
            SJFqueue.push(processes[i]);
            currentTime = processes[i].getArrivalTime();
            if (currentTime <= 9999) {
                std::cout << "time " << currentTime << "ms: Process " << processes[i].getId() << " (tau " << processes[i].getTau() << "ms) arrived; added to ready queue " << printQueue(SJFqueue) << std::endl;
            }
            processes[i].setWaiting(currentTime);
            i++;
        }
    }

    // After the loop, ensure all remaining processes are terminated and the simulation ends
    while (!processes.empty()) {
        int finalTime = currentTime;
        if (currentTime >= 10000) {
            finalTime = processes[0].getArrivalTime();
        }
        std::cout << "time " << finalTime << "ms: Process " << processes[0].getId() << " terminated [Q empty]" << std::endl;
        processes.erase(processes.begin());
    }

    std::cout << "time " << currentTime << "ms: Simulator ended for SJF [Q empty]" << std::endl;

    FILE *fp;
    fp = fopen("simout.txt", "a");

    double utilization = 0;
    if (currentTime != 0) {
        utilization = ceil(((cpuBoundBurstTime + ioBoundBurstTime) / currentTime) * 100000) / 1000;
    }

    double cpuBoundAvgWait = 0, cpuBoundAvgTurn = 0;
    if (numCpuBoundBursts != 0) {
        cpuBoundAvgWait = ceil((cpuBoundWaitTime / numCpuBoundBursts) * 1000) / 1000;
        cpuBoundAvgTurn = ceil(((cpuBoundWaitTime + cpuBoundBurstTime + (cpuSwitches * switchTime)) / numCpuBoundBursts) * 1000) / 1000;
    }

    double ioBoundAvgWait = 0, ioBoundAvgTurn = 0;
    if (numIoBoundBursts != 0) {
        ioBoundAvgWait = ceil((ioBoundWaitTime / numIoBoundBursts) * 1000) / 1000;
        ioBoundAvgTurn = ceil(((ioBoundWaitTime + ioBoundBurstTime + (ioSwitches * switchTime)) / numIoBoundBursts) * 1000) / 1000;
    }

    double totalAvgWait = 0, totalAvgTurn = 0;
    if (numCpuBoundBursts + numIoBoundBursts != 0) {
        totalAvgWait = ceil(((cpuBoundWaitTime + ioBoundWaitTime) / (numCpuBoundBursts + numIoBoundBursts)) * 1000) / 1000;
        totalAvgTurn = ceil((((cpuBoundWaitTime + cpuBoundBurstTime + (cpuSwitches * switchTime)) + (ioBoundWaitTime + ioBoundBurstTime + (ioSwitches * switchTime))) / (numCpuBoundBursts + numIoBoundBursts)) * 1000) / 1000;
    }

    fprintf(fp, "\nAlgorithm SJF\n");
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
}
