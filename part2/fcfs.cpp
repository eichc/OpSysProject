#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <cstdio>
#include "Process.h"

using namespace std;

string printQueue(queue<Process> q) {
    string result = "[Q ";
    if (q.empty()) {
        result.append("empty");
    } else {
        queue<Process> temp = q;
        while (!temp.empty()) {
            result.append(temp.front().getId()).append(" ");
            temp.pop();
        }
    }
    result.append("]");
    return result;
}

void fcfs(vector<Process> allP) {
    queue<Process> q;
    int time = 0;
    printf("time %dms: Simulator started for FCFS %s\n", time, printQueue);

    Process* current = nullptr;
    int i = 0, cpuCompleteTime, newArrivalTime;
    bool isCpuComplete = false, canStartCpu = false, isNewArrival = false;
    while (!allP.empty()) {
        //cpu burst completion
        if (current) {
            cpuCompleteTime = current->getFrontCPU() + time;
            isCpuComplete = true;
        } else if (!q.empty()) { //start using cpu
            canStartCpu = true;
        }
        //IO burst completion
        
        //new process arrives
        if (i < allP.size()) {
            newArrivalTime = allP[i].getArrivalTime();
            isNewArrival = true;
        }
    }
}