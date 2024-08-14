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


}