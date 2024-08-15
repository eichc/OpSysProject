#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "Process.h"

using namespace std;
/*
// Helper function to calculate the ceiling value of a double
int ceiling(double value) {
    return static_cast<int>(ceil(value));
}

// Define the comparator for the priority queue (min-heap) based on remaining time
struct CompareProcess {
    bool operator()(const Process &p1, const Process &p2) {
        return p1.getTau() > p2.getTau() || (p1.getTau() == p2.getTau() && p1.getId() > p2.getId());
    }
};

// Function to simulate the SRT scheduling algorithm
void srt(vector<Process> allP, int switchTime) {
    // Configuration and initialization
    int currentTime = 0;
    priority_queue<Process, vector<Process>, CompareProcess> readyQueue;
    vector<Process> processes = allP;
    vector<Process> ioQueue;
    bool cpuIdle = true;
    Process* currentProcess = nullptr;

    // Sort processes by arrival time initially
    sort(processes.begin(), processes.end(), [](const Process& p1, const Process& p2) {
        return p1.getArrivalTime() < p2.getArrivalTime();
    });

    // Print simulation start
    cout << "time " << currentTime << "ms: Simulator started for SRT [Q empty]" << endl;

    // Main simulation loop
    while (!processes.empty() || !readyQueue.empty() || currentProcess != nullptr || !ioQueue.empty()) {
        // Handle process arrivals
        while (!processes.empty() && processes.front().getArrivalTime() <= currentTime) {
            Process p = processes.front();
            processes.erase(processes.begin());
            readyQueue.push(p);
            cout << "time " << currentTime << "ms: Process " << p.getId() << " (tau " << p.getTau() << "ms) arrived; added to ready queue [Q ";
            // Output the state of the ready queue
            priority_queue<Process, vector<Process>, CompareProcess> tempQueue = readyQueue;
            while (!tempQueue.empty()) {
                cout << tempQueue.top().getId() << " ";
                tempQueue.pop();
            }
            cout << "]" << endl;
        }

        // Handle I/O completions
        for (auto it = ioQueue.begin(); it != ioQueue.end();) {
            if (it->getArrivalTime() <= currentTime) {
                Process p = *it;
                it = ioQueue.erase(it);
                readyQueue.push(p);
                cout << "time " << currentTime << "ms: Process " << p.getId() << " (tau " << p.getTau() << "ms) completed I/O; added to ready queue [Q ";
                // Output the state of the ready queue
                priority_queue<Process, vector<Process>, CompareProcess> tempQueue = readyQueue;
                while (!tempQueue.empty()) {
                    cout << tempQueue.top().getId() << " ";
                    tempQueue.pop();
                }
                cout << "]" << endl;
            } else {
                ++it;
            }
        }

        // If CPU is idle, select a process from the ready queue
        if (cpuIdle && !readyQueue.empty()) {
            currentProcess = new Process(readyQueue.top());
            readyQueue.pop();
            cpuIdle = false;
            cout << "time " << currentTime << "ms: Process " << currentProcess->getId() << " (tau " << currentProcess->getTau() << "ms) started using the CPU for " << currentProcess->getFrontCPU() << "ms burst [Q ";
            // Output the state of the ready queue
            priority_queue<Process, vector<Process>, CompareProcess> tempQueue = readyQueue;
            while (!tempQueue.empty()) {
                cout << tempQueue.top().getId() << " ";
                tempQueue.pop();
            }
            cout << "]" << endl;
        }

        // Process running on CPU
        if (currentProcess != nullptr) {
            int burstTime = currentProcess->getFrontCPU();
            if (currentProcess->isEmptyCPU()) {
                // Process finished all CPU bursts
                cout << "time " << currentTime + burstTime << "ms: Process " << currentProcess->getId() << " completed a CPU burst; " << currentProcess->getRemainingBursts() << " bursts to go [Q ";
                // Output the state of the ready queue
                priority_queue<Process, vector<Process>, CompareProcess> tempQueue = readyQueue;
                while (!tempQueue.empty()) {
                    cout << tempQueue.top().getId() << " ";
                    tempQueue.pop();
                }
                cout << "]" << endl;
                delete currentProcess;
                currentProcess = nullptr;
                cpuIdle = true;
            } else {
                // Process is still running
                currentProcess->popFrontCPU();
                cout << "time " << currentTime + burstTime << "ms: Process " << currentProcess->getId() << " (tau " << currentProcess->getTau() << "ms) completed a CPU burst; " << currentProcess->getRemainingBursts() << " bursts to go [Q ";
                // Output the state of the ready queue
                priority_queue<Process, vector<Process>, CompareProcess> tempQueue = readyQueue;
                while (!tempQueue.empty()) {
                    cout << tempQueue.top().getId() << " ";
                    tempQueue.pop();
                }
                cout << "]" << endl;
                currentProcess->setTau(ceiling(currentProcess->getTau() * 0.75)); // Example alpha value
                // Process starts I/O
                cout << "time " << currentTime + burstTime << "ms: Process " << currentProcess->getId() << " switching out of CPU; blocking on I/O until time " << currentTime + burstTime + 1000 << "ms [Q ";
                // Output the state of the ready queue
                tempQueue = readyQueue;
                while (!tempQueue.empty()) {
                    cout << tempQueue.top().getId() << " ";
                    tempQueue.pop();
                }
                cout << "]" << endl;
                ioQueue.push_back(*currentProcess);
                delete currentProcess;
                currentProcess = nullptr;
                cpuIdle = true;
            }
            currentTime += burstTime;
        } else {
            // CPU is idle and no processes are available
            currentTime += 10; // Arbitrary idle time increment
        }
    }

    // Print simulation end
    cout << "time " << currentTime << "ms: Simulator ended for SRT [Q empty]" << endl;
}
*/