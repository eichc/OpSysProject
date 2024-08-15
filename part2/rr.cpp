#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <cstdio>
#include <limits.h>
#include "Process.h"

using namespace std;

extern string printQueue(queue<Process> q);

int rr(vector<Process> allP, int switchTime, int slice) {
    queue<Process> q;
    int time = 0;
    int preemptions = 0;
    cout << "time " << time << "ms: Simulator started for RR " << printQueue(q) << endl;

    int current = -1;
    unsigned int i = 0;
    int cpuCompleteTime, newArrivalTime, blockingTime, cpuStartTime, preemptTime, nextStartTime = -1;
    bool isCpuComplete, canStartCpu, isIOComplete, isNewArrival, isPreemption;
    while (!allP.empty()) {
        blockingTime = cpuStartTime = newArrivalTime = INT_MAX;
        isCpuComplete = canStartCpu = isIOComplete = isNewArrival = isPreemption = false;
        //cpu burst completion
        if (current != -1) {
            if (preemptTime == -1 || cpuCompleteTime == -1) {
                cerr << "ERROR: Preempt time and Complete time both -1" << endl;
                return -1;
            } else if (cpuCompleteTime < preemptTime) {
                isCpuComplete = true;
            } else {
                isPreemption = true;
            }
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

        if (!isCpuComplete && !canStartCpu && !isIOComplete && !isNewArrival && !isPreemption) {
            cerr << "ERROR: Nothing is able to be done\n";
            return -1;
        }

        //complete the cpu burst
        if ((isCpuComplete) && min(cpuCompleteTime, blockingTime) == min(cpuCompleteTime, newArrivalTime)) {
            time = cpuCompleteTime;
            allP[current].popFrontCPU();
            if (allP[current].isEmptyCPU()) {
                cout << "time " << time << "ms: Process " << allP[current].getId() << " terminated " << printQueue(q) << endl;
                auto itr = allP.begin();
                while (itr->getId().compare(allP[current].getId()) != 0) {
                    itr++;
                }
                allP.erase(itr);
            } else {
                cout << "time " << time << "ms: Process " << allP[current].getId() << " completed a CPU burst; " << allP[current].getNumCPU() << " bursts to go " << printQueue(q) << endl;
                allP[current].setBlocking(time + allP[current].popFrontIO() + switchTime/2);
                cout << "time " << time << "ms: Process " << allP[current].getId() << " switching out of CPU; blocking on I/O until time " << allP[current].getBlocking() << "ms " << printQueue(q) << endl;
            }
            time += switchTime/2;
            current = -1;
            cpuCompleteTime = preemptTime = INT_MAX;
            if (!q.empty()) {
                nextStartTime = time + switchTime/2;
            } else {
                nextStartTime = -1;
            }
            
        } else if ((isPreemption) && min(preemptTime, blockingTime) == min(preemptTime, newArrivalTime)) { //preemption
            time = preemptTime;
            int temp = allP[current].popFrontCPU();
            allP[current].pushFrontCPU(temp - slice);
            if (q.empty()) {
                cout << "time " << time << "ms: Time slice expired; no preemption because ready queue is empty " << printQueue(q) << endl;
                preemptTime = time + slice;
            } else {
                cout << "time " << time << "ms: Time slice expired; preempting process " << allP[current].getId() << " with " << allP[current].getFrontCPU() << "ms remaining " << printQueue(q) << endl;
                q.push(allP[current]);
                time += switchTime/2;
                current = -1;
                cpuCompleteTime = preemptTime = INT_MAX;
                if (!q.empty()) {
                    nextStartTime = time + switchTime/2;
                } else {
                    nextStartTime = -1;
                }
                preemptions++;
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
            int temp = allP[current].getFrontCPU();
            cout << "time " << time << "ms: Process " << allP[current].getId() << " started using the CPU for " << temp << "ms burst " << printQueue(q) << endl;
            preemptTime = time + slice;
            cpuCompleteTime = time + temp;

        } else if (blockingTime < newArrivalTime) { //complete the IO burst
            time = blockingTime;
            for (unsigned int j = 0; j < allP.size(); j++) {
                if (allP[j].getBlocking() == time) {
                    allP[j].setBlocking(-1);
                    q.push(allP[j]);
                    cout << "time " << time << "ms: Process " << allP[j].getId() << " completed I/O; added to ready queue " << printQueue(q) << endl;
                    break;
                }
            }
            
        } else { //new process arrives
            q.push(allP[i]);
            time = allP[i].getArrivalTime();
            cout << "time " << time << "ms: Process " << allP[i].getId() << " arrived; added to ready queue " << printQueue(q) << endl;
            i++;
        }
    }
    
    cout << "time " << time << "ms: Simulator ended for RR " << printQueue(q) << endl;

    return 0;
}