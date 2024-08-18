#include <queue>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <limits.h>
#include <fstream>  
#include "Process.h"

extern std::string printQueue(std::priority_queue<Process> q);

void srt(std::vector<Process> processes, int switchTime, double lambda, double alpha) {
    std::priority_queue<Process> SRTqueue;
    int currentTime = 0;  
    int current = -1; 
    int cpuCompleteTime = INT_MAX;
    unsigned int i = 0;
    int blockingTime, cpuStartTime, newArrivalTime, nextSwitchTime = -1, switchingIn = -1, cpuSwitchTime = 0;
    bool isCpuComplete, canStartCpu, isIOComplete, isNewArrival, canSwitchIn;

    // Variables to track statistics
    double cpuBoundWaitTime = 0, ioBoundWaitTime = 0;
    double cpuBoundTurnaroundTime = 0, ioBoundTurnaroundTime = 0;
    double cpuBoundBurstTime = 0, ioBoundBurstTime = 0;
    int cpuSwitches = 0, ioSwitches = 0;
    int numCpuBoundBursts = 0, numIoBoundBursts = 0;
    double cpuBusyTime = 0; 
    int cpuPreemption=0;
    int ioPreemption=0;

    std::cout << "time 0ms: Simulator started for SRT [Q empty]" << std::endl;

    // Calculate the initial tau based on the given lambda
    int initialTau = std::ceil(1 / lambda);
    
    // Initialize tau for each process
    for (auto& process : processes) {
        process.setTau(initialTau);
        process.setPredictedRemaining(initialTau);
    }

    while (!processes.empty()) {
        blockingTime = newArrivalTime = INT_MAX;
        isCpuComplete = canStartCpu = isIOComplete = isNewArrival = canSwitchIn = false;

        // CPU burst completion
        if (current != -1) {
            isCpuComplete = true;
        } else if (switchingIn != -1) { // Start using CPU
            canStartCpu = true;
            cpuStartTime = cpuSwitchTime + switchTime/2;
        } else if (!SRTqueue.empty()) { // Switch into CPU
            canSwitchIn = true;
            cpuSwitchTime = std::max(currentTime, nextSwitchTime);
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

        if (!isCpuComplete && !canStartCpu && !isIOComplete && !isNewArrival && !canSwitchIn) {
            std::cerr << "ERROR: Nothing is able to be done\n";
            return;
        }

        // Complete the CPU burst
        if (isCpuComplete && cpuCompleteTime <= blockingTime && cpuCompleteTime <= newArrivalTime) {
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

            processes[current].setRemaining(-1);
            if (!processes[current].isEmptyCPU()) {
                #ifndef DEBUG_MODE_SRT
                if (currentTime <= 9999) {
                #endif
                    if (processes[current].getNumCPU() == 1) {
                        std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " (tau " << processes[current].getTau() << "ms) completed a CPU burst; " << processes[current].getNumCPU() << " burst to go " << printQueue(SRTqueue) << std::endl;
                    } else {
                        std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " (tau " << processes[current].getTau() << "ms) completed a CPU burst; " << processes[current].getNumCPU() << " bursts to go " << printQueue(SRTqueue) << std::endl;
                    }
                #ifndef DEBUG_MODE_SRT
                }
                #endif

                int oldTau = processes[current].getTau();
                int newTau = std::ceil((alpha * burstTime) + ((1 - alpha) * oldTau));
                processes[current].setTau(newTau);
                processes[current].setPredictedRemaining(newTau);

                #ifndef DEBUG_MODE_SRT
                if (currentTime <= 9999) {
                #endif
                    std::cout << "time " << currentTime << "ms: Recalculated tau for process " << processes[current].getId() 
                              << ": old tau " << oldTau << "ms ==> new tau " << newTau << "ms " << printQueue(SRTqueue) << std::endl;
                #ifndef DEBUG_MODE_SRT
                }
                #endif

                processes[current].setBlocking(currentTime + processes[current].popFrontIO() + switchTime / 2);
                #ifndef DEBUG_MODE_SRT
                if (currentTime <= 9999) {
                #endif
                    std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " switching out of CPU; blocking on I/O until time " << processes[current].getBlocking() << "ms " << printQueue(SRTqueue) << std::endl;
                #ifndef DEBUG_MODE_SRT
                }
                #endif
            } else {
                std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " terminated " << printQueue(SRTqueue) << std::endl;
                processes.erase(processes.begin() + current);
            }

            currentTime += switchTime / 2;
            current = -1;
            cpuCompleteTime = INT_MAX;
            nextSwitchTime = currentTime;

        } else if (canSwitchIn && cpuSwitchTime < blockingTime && cpuSwitchTime < newArrivalTime) { //switch into CPU
            currentTime = cpuSwitchTime;
            for (unsigned int j = 0; j < processes.size(); j++) {
                if (processes[j].getId().compare(SRTqueue.top().getId()) == 0) {
                    switchingIn = j;
                    break;
                }
            }
            SRTqueue.pop();
            if (processes[switchingIn].isCpuBound()) {
                cpuBoundWaitTime += (currentTime - processes[switchingIn].getWaiting());
                cpuSwitches++;
            } else {
                ioBoundWaitTime += (currentTime - processes[switchingIn].getWaiting());
                ioSwitches++;
            }

        } else if (canStartCpu && cpuStartTime <= blockingTime && cpuStartTime <= newArrivalTime) { // Start next CPU burst
            // for (unsigned int j = 0; j < processes.size(); j++) {
            //     if (processes[j].getId().compare(SRTqueue.top().getId()) == 0) {
            //         current = j;
            //         break;
            //     }
            // }
            // SRTqueue.pop();
            // if (nextSwitchTime < currentTime) {
            //     currentTime += switchTime/2;
            // } else {
            //     currentTime = nextSwitchTime;
            // }
            currentTime = cpuStartTime;
            current = switchingIn;
            switchingIn = -1;
            int temp;
            
            // if (processes[current].isCpuBound()) {
            //     cpuBoundWaitTime += (currentTime - processes[current].getWaiting() - switchTime / 2);
            //     cpuSwitches++;
            // } else {
            //     ioBoundWaitTime += (currentTime - processes[current].getWaiting() - switchTime / 2);
            //     ioSwitches++;
            // }
            processes[current].setStart(currentTime);
            
                if (processes[current].getRemaining() != -1) {
                    temp = processes[current].getRemaining();
                    #ifndef DEBUG_MODE_SRT
                    if (currentTime <= 9999) {
                    #endif
                        std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " (tau " << processes[current].getTau() << "ms) started using the CPU for remaining " << temp << "ms of " << processes[current].getFrontCPU() << "ms burst " << printQueue(SRTqueue) << std::endl;
                    #ifndef DEBUG_MODE_SRT
                    }
                    #endif
                } else {
                    temp = processes[current].getFrontCPU();
                    #ifndef DEBUG_MODE_SRT
                    if (currentTime <= 9999) {
                    #endif
                        std::cout << "time " << currentTime << "ms: Process " << processes[current].getId() << " (tau " << processes[current].getTau() << "ms) started using the CPU for " << temp << "ms burst " << printQueue(SRTqueue) << std::endl;
                    #ifndef DEBUG_MODE_SRT
                    }
                    #endif
                }
            cpuCompleteTime = currentTime + temp;
        } else if (blockingTime < newArrivalTime) { // Complete the I/O burst
            currentTime = blockingTime;
            for (unsigned int j = 0; j < processes.size(); j++) {
                if (processes[j].getBlocking() == currentTime) {
                    processes[j].setBlocking(-1);
                    SRTqueue.push(processes[j]);
                    processes[j].setWaiting(currentTime);
                    //check for preemption
                    if (current != -1 && processes[j].getTau() < processes[current].getPredictedRemaining() - (currentTime - processes[current].getStart())) {
                        int ran = currentTime - processes[current].getStart();
                        if (processes[current].getRemaining() == -1) {
                            #ifndef DEBUG_MODE_SRT
                            if (currentTime <= 9999) {
                            #endif
                                std::cout << "time " << currentTime << "ms: Process " << processes[j].getId() <<" (tau " << processes[j].getTau() << "ms) completed I/O; preempting " << processes[current].getId() << " (predicted remaining time " << processes[current].getPredictedRemaining() - ran << "ms) " << printQueue(SRTqueue) << std::endl;
                            #ifndef DEBUG_MODE_SRT
                            }
                            #endif
                            processes[current].setRemaining(processes[current].getFrontCPU() - ran);
                            processes[current].setPredictedRemaining(processes[current].getTau() - ran);
                        } else {
                            #ifndef DEBUG_MODE_SRT
                            if (currentTime <= 9999) {
                            #endif
                                std::cout << "time " << currentTime << "ms: Process " << processes[j].getId() <<" (tau " << processes[j].getTau() << "ms) completed I/O; preempting " << processes[current].getId() << " (predicted remaining time " << processes[current].getPredictedRemaining() - ran << "ms) " << printQueue(SRTqueue) << std::endl;
                            #ifndef DEBUG_MODE_SRT
                            }
                            #endif
                            processes[current].setPredictedRemaining(processes[current].getPredictedRemaining() - ran);
                            processes[current].setRemaining(processes[current].getRemaining() - ran);
                        }
                        currentTime += switchTime/2;
                        SRTqueue.push(processes[current]);
                        processes[current].setWaiting(currentTime);
                        if (processes[current].isCpuBound()) {
                            cpuPreemption++;
                        } else {
                            ioPreemption++;
                        }
                        current = -1;
                        cpuCompleteTime = INT_MAX;
                        nextSwitchTime = currentTime;
                    } else { //no preemption
                        #ifndef DEBUG_MODE_SRT
                        if (currentTime <= 9999) {
                        #endif
                            std::cout << "time " << currentTime << "ms: Process " << processes[j].getId() <<" (tau " << processes[j].getTau() << "ms) completed I/O; added to ready queue " << printQueue(SRTqueue) << std::endl;
                        
                        #ifndef DEBUG_MODE_SRT
                        }
                        #endif
                    }
                    break;
                }
            }
            
        } else { // New process arrives
            SRTqueue.push(processes[i]);
            currentTime = processes[i].getArrivalTime();
            #ifndef DEBUG_MODE_SRT
            if (currentTime <= 9999) {
            #endif
                std::cout << "time " << currentTime << "ms: Process " << processes[i].getId() << " (tau " << processes[i].getTau() << "ms) arrived; added to ready queue " << printQueue(SRTqueue) << std::endl;
            #ifndef DEBUG_MODE_SRT
            }
            #endif
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

    std::cout << "time " << currentTime << "ms: Simulator ended for SRT [Q empty]" << std::endl;

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
    
    fprintf(fp, "\nAlgorithm SRT\n");
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
    fprintf(fp, "-- CPU-bound number of preemptions: %d\n", cpuPreemption); // Ensure you count preemptions accurately
    fprintf(fp, "-- I/O-bound number of preemptions: %d\n", ioPreemption); // Ensure you count preemptions accurately
    fprintf(fp, "-- overall number of preemptions: %d\n", cpuPreemption + ioPreemption); // Ensure you count preemptions accurately

    fclose(fp);
}
