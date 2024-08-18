#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <cstdio>
#include <limits.h>
#include "Process.h"

using namespace std;

class Compare {
public:
    bool operator()(Process below, Process above) {
        if (below.getTimeAdded() >= above.getTimeAdded()) {
            return true;
        }
        return false;
    }
};

string printQueue(priority_queue<Process, vector<Process>, Compare> q) {
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

extern string printQueue(queue<Process> q);

int rr(vector<Process> allP, int switchTime, int slice) {
    //priority_queue<Process, vector<Process>, Compare> q;
    queue<Process> q;
    int time = 0;
    cout << "time " << time << "ms: Simulator started for RR " << printQueue(q) << endl;

    //simout variables
    double cpuBoundWaitTime = 0, ioBoundWaitTime = 0, cpuBoundBurstTime = 0, ioBoundBurstTime = 0, cpuSingleSlice = 0, ioSingleSlice = 0;
    int cpuSwitches = 0, ioSwitches = 0, numCpuBoundBursts = 0, numIoBoundBursts = 0, ioPreemptions = 0, cpuPreemptions = 0;

    int current = -1;
    unsigned int i = 0;
    int switchOutTime = 0, newArrivalTime, blockingTime, cpuStartTime, preemptTime, switchingIn = -1, switchInTime = 0;
    int switchingOut = -1, cpuCompleteTime;
    bool isCpuComplete, canStartCpu, isIOComplete, isNewArrival, isPreemption, canSwitchIn, canSwitchOut;
    while (!allP.empty()) {
        blockingTime = newArrivalTime = INT_MAX;
        isCpuComplete = canStartCpu = isIOComplete = isNewArrival = isPreemption = canSwitchIn = canSwitchOut = false;
        //cpu burst completion
        if (current != -1) {
            if (switchingOut != -1) {
                canSwitchOut = true;
            } else if (cpuCompleteTime <= preemptTime) {
                isCpuComplete = true;
            } else {
                isPreemption = true;
            }
        } else if (switchingIn != -1) { // Start using CPU
            canStartCpu = true;
            cpuStartTime = switchInTime + switchTime/2;
        } else if (!q.empty()) { // Switch into CPU
            canSwitchIn = true;
            switchInTime = max(time, switchOutTime);
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

        if (!isCpuComplete && !canStartCpu && !isIOComplete && !isNewArrival && !isPreemption && !canSwitchIn && !canSwitchOut) {
            cerr << "ERROR: Nothing is able to be done\n";
            return -1;
        }

        //complete the cpu burst
        if ((isCpuComplete) && cpuCompleteTime <= blockingTime && cpuCompleteTime <= newArrivalTime) {
            time = cpuCompleteTime;
            if (allP[current].isCpuBound()) {
                if (allP[current].getFrontCPU() <= slice) {
                    cpuSingleSlice++;
                }
                cpuBoundBurstTime += allP[current].popFrontCPU();
                numCpuBoundBursts++;
            } else {
                if (allP[current].getFrontCPU() <= slice) {
                    ioSingleSlice++;
                }
                ioBoundBurstTime += allP[current].popFrontCPU();
                numIoBoundBursts++;
            }
            allP[current].setRemaining(-1);
            if (allP[current].isEmptyCPU()) {
                cout << "time " << time << "ms: Process " << allP[current].getId() << " terminated " << printQueue(q) << endl;
                auto itr = allP.begin();
                while (itr->getId().compare(allP[current].getId()) != 0) {
                    itr++;
                }
                allP.erase(itr);
            } else {
                if (allP[current].getNumCPU() == 1) {
                    #ifndef DEBUG_MODE_RR
                    if (time < 10000) {
                    #endif
                        cout << "time " << time << "ms: Process " << allP[current].getId() << " completed a CPU burst; " << allP[current].getNumCPU() << " burst to go " << printQueue(q) << endl;
                    #ifndef DEBUG_MODE_RR
                    }
                    #endif
                } else {
                    #ifndef DEBUG_MODE_RR
                    if (time < 10000) {
                    #endif
                        cout << "time " << time << "ms: Process " << allP[current].getId() << " completed a CPU burst; " << allP[current].getNumCPU() << " bursts to go " << printQueue(q) << endl;
                    #ifndef DEBUG_MODE_RR
                    }
                    #endif
                }
                allP[current].setBlocking(time + allP[current].popFrontIO() + switchTime/2);
                #ifndef DEBUG_MODE_RR
                if (time < 10000) {
                #endif
                     cout << "time " << time << "ms: Process " << allP[current].getId() << " switching out of CPU; blocking on I/O until time " << allP[current].getBlocking() << "ms " << printQueue(q) << endl;
                #ifndef DEBUG_MODE_RR
                }
                #endif
            }
            switchingOut = current;
            switchOutTime = time + switchTime/2;
            
        } else if ((isPreemption) && preemptTime <= blockingTime && preemptTime <= newArrivalTime) { //preemption
            time = preemptTime;
            if (allP[current].getRemaining() == -1) {
                allP[current].setRemaining(allP[current].getFrontCPU() - slice);
            } else {
                allP[current].setRemaining(allP[current].getRemaining() - slice);
            }
            if (q.empty()) {
                #ifndef DEBUG_MODE_RR
                if (time < 10000) {
                #endif
                    cout << "time " << time << "ms: Time slice expired; no preemption because ready queue is empty " << printQueue(q) << endl;
                #ifndef DEBUG_MODE_RR
                }
                #endif
                preemptTime = time + slice;
            } else {
                #ifndef DEBUG_MODE_RR
                if (time < 10000) {
                #endif
                    cout << "time " << time << "ms: Time slice expired; preempting process " << allP[current].getId() << " with " << allP[current].getRemaining() << "ms remaining " << printQueue(q) << endl;
                #ifndef DEBUG_MODE_RR
                }
                #endif
                allP[current].setTimeAdded(time);
                // q.push(allP[current]);
                // allP[current].setWaiting(time);
                if (allP[current].isCpuBound()) {
                    cpuPreemptions++;
                } else {
                    ioPreemptions++;
                }
                switchingOut = current;
                switchOutTime = time + switchTime/2;
            }

        } else if (canSwitchOut && switchOutTime <= blockingTime && switchOutTime <= newArrivalTime) { //Switch out of CPU
            time = switchOutTime;
            if (time == cpuCompleteTime + switchTime/2) { //cpu completed
                ;
            } else { //was preempted
                q.push(allP[current]);
                allP[current].setWaiting(time);
            }
            current = -1;
            switchingOut = -1;
            cpuCompleteTime = preemptTime = INT_MAX;

        } else if (canSwitchIn && switchInTime < blockingTime && switchInTime < newArrivalTime) { //Switch into CPU
            time = switchInTime;
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
            // for (unsigned int j = 0; j < allP.size(); j++) {
            //     if (allP[j].getId().compare(q.top().getId()) == 0) {
            //         current = j;
            //         break;
            //     }
            // }
            // q.pop();
            time = cpuStartTime;
            current = switchingIn;
            switchingIn = -1;
            // if (allP[current].isCpuBound()) {
            //     cpuBoundWaitTime += (time - allP[current].getWaiting() - switchTime/2);
            //     cpuSwitches++;
            // } else {
            //     ioBoundWaitTime += (time - allP[current].getWaiting() - switchTime/2);
            //     ioSwitches++;
            // }
            int temp;
            if (allP[current].getRemaining() != -1) {
                temp = allP[current].getRemaining();
                #ifndef DEBUG_MODE_RR
                if (time < 10000) {
                #endif
                    cout << "time " << time << "ms: Process " << allP[current].getId() << " started using the CPU for remaining " << temp << "ms of " << allP[current].getFrontCPU() << "ms burst " << printQueue(q) << endl;
                #ifndef DEBUG_MODE_RR
                }
                #endif
            } else {
                temp = allP[current].getFrontCPU();
                #ifndef DEBUG_MODE_RR
                if (time < 10000) {
                #endif
                    cout << "time " << time << "ms: Process " << allP[current].getId() << " started using the CPU for " << temp << "ms burst " << printQueue(q) << endl;
                #ifndef DEBUG_MODE_RR
                }
                #endif
            }
            preemptTime = time + slice;
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
            allP[selector].setTimeAdded(time);
            q.push(allP[selector]);
            #ifndef DEBUG_MODE_RR
            if (time < 10000) {
            #endif
                cout << "time " << time << "ms: Process " << allP[selector].getId() << " completed I/O; added to ready queue " << printQueue(q) << endl;
            #ifndef DEBUG_MODE_RR
            }
            #endif
            allP[selector].setWaiting(time);
            
        } else { //new process arrives
            q.push(allP[i]);
            time = allP[i].getArrivalTime();
            #ifndef DEBUG_MODE_RR
            if (time < 10000) {
            #endif
                cout << "time " << time << "ms: Process " << allP[i].getId() << " arrived; added to ready queue " << printQueue(q) << endl;
            #ifndef DEBUG_MODE_RR
            }
            #endif
            allP[i].setWaiting(time);
            i++;
        }
    }
    time+= switchTime/2;
    cout << "time " << time << "ms: Simulator ended for RR " << printQueue(q) << endl;

    //simout
    FILE *fp;
    fp = fopen("simout.txt", "a");

    double utilization = 0;
    if (time != 0) {
        utilization = ceil(((cpuBoundBurstTime + ioBoundBurstTime)/time) * 100000) / 1000;
    }
    double cpuBoundAvgWait = 0, cpuBoundAvgTurn = 0, percentCpuSingleSlice = 0;
    if (numCpuBoundBursts != 0) {
        cpuBoundAvgWait = ceil((cpuBoundWaitTime/numCpuBoundBursts) * 1000) / 1000;
        cpuBoundAvgTurn = ceil(( (cpuBoundWaitTime+cpuBoundBurstTime+(cpuSwitches*switchTime)) / numCpuBoundBursts) * 1000) / 1000;
        percentCpuSingleSlice = ceil((cpuSingleSlice/numCpuBoundBursts) * 100000) / 1000;
    }
    double ioBoundAvgWait = 0, ioBoundAvgTurn = 0, percentIoSingleSlice = 0;
    if (numIoBoundBursts != 0) {
        ioBoundAvgWait = ceil((ioBoundWaitTime/numIoBoundBursts) * 1000) / 1000;
        ioBoundAvgTurn = ceil(( (ioBoundWaitTime+ioBoundBurstTime+(ioSwitches*switchTime)) / numIoBoundBursts) * 1000) / 1000;
        percentIoSingleSlice = ceil((ioSingleSlice/numIoBoundBursts) * 100000) / 1000;
    }
    double totalAvgWait = 0, totalAvgTurn = 0, totalSingleSlice = 0;
    if (numCpuBoundBursts + numIoBoundBursts != 0) {
        totalAvgWait = ceil(((cpuBoundWaitTime + ioBoundWaitTime)/(numCpuBoundBursts + numIoBoundBursts)) * 1000) / 1000;
        totalAvgTurn = ceil((((cpuBoundWaitTime+cpuBoundBurstTime+(cpuSwitches*switchTime)) + (ioBoundWaitTime+ioBoundBurstTime+(ioSwitches*switchTime)))/(numCpuBoundBursts + numIoBoundBursts)) * 1000) / 1000;
        totalSingleSlice = ceil(((cpuSingleSlice + ioSingleSlice)/(numCpuBoundBursts + numIoBoundBursts)) * 100000) / 1000;
    }

    fprintf(fp, "\nAlgorithm RR\n");
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
    fprintf(fp, "-- CPU-bound number of preemptions: %d\n", cpuPreemptions);
    fprintf(fp, "-- I/O-bound number of preemptions: %d\n", ioPreemptions);
    fprintf(fp, "-- overall number of preemptions: %d\n", cpuPreemptions + ioPreemptions);
    fprintf(fp, "-- CPU-bound percentage of CPU bursts completed within one time slice: %.3f%%\n", percentCpuSingleSlice);
    fprintf(fp, "-- I/O-bound percentage of CPU bursts completed within one time slice: %.3f%%\n", percentIoSingleSlice);
    fprintf(fp, "-- overall percentage of CPU bursts completed within one time slice: %.3f%%\n", totalSingleSlice);

    fclose(fp);

    return 0;
}