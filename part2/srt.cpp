#include <queue>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <limits.h>
#include "Process.h"

std::string printQueueSRT(std::priority_queue<Process> q) {
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

void srt(std::vector<Process> processes, int switchTime, double lambda, double alpha) {
    std::priority_queue<Process> SRTqueue;
    int currentTime = 0;
    int current = -1;  // Index of the current process in the CPU
    int cpuCompleteTime = INT_MAX;
    unsigned int i = 0;
    int blockingTime, cpuStartTime, newArrivalTime;
    bool isCpuComplete, canStartCpu, isIOComplete, isNewArrival, isPreemption;

    std::cout << "time 0ms: Simulator started for SRT [Q empty]" << std::endl;

    // Calculate the initial tau based on the given lambda
    int initialTau = std::ceil(1 / lambda);

    // Initialize tau for each process
    for (auto& process : processes) {
        process.setTau(initialTau);
    }

    while (!processes.empty()) {
        blockingTime = cpuStartTime = newArrivalTime = INT_MAX;
        isCpuComplete = canStartCpu = isIOComplete = isNewArrival = isPreemption = false;

        // CPU burst completion
        if (current != -1 && cpuCompleteTime < INT_MAX) {
            isCpuComplete = true;
        } else if (!SRTqueue.empty()) {  // Start using CPU
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

        if (!isCpuComplete && !canStartCpu && !isIOComplete && !isNewArrival && !isPreemption) {
            std::cerr << "ERROR: Nothing is able to be done\n";
            return;
        }

        // Complete the CPU burst
        if (isCpuComplete && std::min(cpuCompleteTime, blockingTime) == std::min(cpuCompleteTime, newArrivalTime)) {
            currentTime = cpuCompleteTime;
            int burstTime = processes[current].popFrontCPU();

            if (!processes[current].isEmptyCPU()) {
                if (currentTime <= 9999) {
                    std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " (tau " << processes[current].getTau() << "ms) completed a CPU burst; " << processes[current].getNumCPU() << " bursts to go " << printQueueSRT(SRTqueue) << std::endl;
                }

                int oldTau = processes[current].getTau();
                int newTau = std::ceil((alpha * burstTime) + ((1 - alpha) * oldTau));
                processes[current].setTau(newTau);

                if (currentTime <= 9999) {
                    std::cout << "time " << currentTime << "ms: Recalculated tau for process " << processes[current].getId()
                              << ": old tau " << oldTau << "ms ==> new tau " << newTau << "ms " << printQueueSRT(SRTqueue) << std::endl;
                }

                processes[current].setBlocking(currentTime + processes[current].popFrontIO() + switchTime / 2);
                if (currentTime <= 9999) {
                    std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " switching out of CPU; blocking on I/O until time " << processes[current].getBlocking() << "ms " << printQueueSRT(SRTqueue) << std::endl;
                }
            } else {
                if (currentTime <= 9999) {
                    std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " terminated " << printQueueSRT(SRTqueue) << std::endl;
                } else {
                    std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " terminated [Q empty]" << std::endl;
                }
                processes.erase(processes.begin() + current);
            }

            currentTime += switchTime / 2;
            current = -1;
            cpuCompleteTime = INT_MAX;

        } else if (canStartCpu && std::min(cpuStartTime, blockingTime) == std::min(cpuStartTime, newArrivalTime)) {
            // Start next CPU burst
            Process nextProcess = SRTqueue.top();
            int remainingTime = processes[current].getFrontCPU() - (cpuCompleteTime - currentTime);

            // Preemption check
            if (current != -1 && nextProcess.getTau() < remainingTime) {
                std::cout << "time " << currentTime << "ms: Process " << nextProcess.getId()
                          << " (tau " << nextProcess.getTau() << "ms) completed I/O; preempting " << processes[current].getId()
                          << " (predicted remaining time " << remainingTime << "ms) " << printQueueSRT(SRTqueue) << std::endl;

                processes[current].pushFrontCPU(remainingTime);
                SRTqueue.push(processes[current]);
                current = -1;
                cpuCompleteTime = INT_MAX;
                canStartCpu = true;
            }

            for (unsigned int j = 0; j < processes.size(); j++) {
                if (processes[j].getId().compare(SRTqueue.top().getId()) == 0) {
                    current = j;
                    break;
                }
            }
            SRTqueue.pop();
            currentTime += switchTime / 2;
            int temp = processes[current].getFrontCPU();
            if (currentTime <= 9999) {
                std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " (tau " << processes[current].getTau() << "ms) started using the CPU for " << temp << "ms burst " << printQueueSRT(SRTqueue) << std::endl;
            }
            cpuCompleteTime = currentTime + temp;
        } else if (blockingTime < newArrivalTime) {
            // Complete the I/O burst
            currentTime = blockingTime;
            for (unsigned int j = 0; j < processes.size(); j++) {
                if (processes[j].getBlocking() == currentTime) {
                    processes[j].setBlocking(-1);
                    SRTqueue.push(processes[j]);
                    if (currentTime <= 9999) {
                        std::cout << "time " << currentTime << "ms: Process " << processes[j].getId() << " (tau " << processes[j].getTau() << "ms) completed I/O; added to ready queue " << printQueueSRT(SRTqueue) << std::endl;
                    }
                    break;
                }
            }
            
        } else {
            // New process arrives
            SRTqueue.push(processes[i]);
            currentTime = processes[i].getArrivalTime();
            if (currentTime <= 9999) {
                std::cout << "time " << currentTime << "ms: Process " << processes[i].getId() << " (tau " << processes[i].getTau() << "ms) arrived; added to ready queue " << printQueueSRT(SRTqueue) << std::endl;
            }
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

    std::cout << "time " << currentTime << "ms: Simulator ended for SRT [Q empty]" << std::endl;
}
