#include <string>
#include <iostream>
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

int fcfs(vector<Process> allP, int switchTime) {
    queue<Process> q;
    int time = 0;
    cout << "time " << time << "ms: Simulator started for FCFS " << printQueue(q) << endl;

    //Process* current = nullptr;
    int current = -1;
    unsigned int i = 0;
    int cpuCompleteTime, newArrivalTime, blockingTime, cpuStartTime;
    bool isCpuComplete, canStartCpu, isIOComplete, isNewArrival;
    while (!allP.empty()) {
        blockingTime = cpuStartTime = newArrivalTime = INT_MAX;
        isCpuComplete = canStartCpu = isIOComplete = isNewArrival = false;
        //cpu burst completion
        if (current != -1) {
            isCpuComplete = true;
        } else if (!q.empty()) { //start using cpu
            canStartCpu = true;
            cpuStartTime = time + switchTime/2;
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
            cpuCompleteTime = INT_MAX;
            
        } else if (canStartCpu && min(cpuStartTime, blockingTime) == min(cpuStartTime, newArrivalTime)) { //start next cpu burst
            for (unsigned int j = 0; j < allP.size(); j++) {
                if (allP[j].getId().compare(q.front().getId()) == 0) {
                    current = j;
                    break;
                }
            }
            q.pop();
            time += switchTime/2;
            int temp = allP[current].getFrontCPU();
            cout << "time " << time << "ms: Process " << allP[current].getId() << " started using the CPU for " << temp << "ms burst " << printQueue(q) << endl;
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
    
    cout << "time " << time << "ms: Simulator ended for FCFS " << printQueue(q) << endl;

    return 0;
}