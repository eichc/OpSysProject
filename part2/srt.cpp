#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip> // for setprecision
#include "Process.h"

using namespace std;
<<<<<<< Updated upstream
/*
// Helper function to calculate the ceiling value of a double
int ceiling(double value) {
    return static_cast<int>(ceil(value));
}

// Define the comparator for the priority queue (min-heap) based on remaining time
=======

// Comparator for priority queue to handle SRT scheduling (shortest remaining time)
>>>>>>> Stashed changes
struct CompareProcess {
    bool operator()(const Process& p1, const Process& p2) {
        if (p1.getRemainingTime() == p2.getRemainingTime()) {
            return p1.getId() > p2.getId(); // Tie-breaking by process ID
        }
        return p1.getRemainingTime() > p2.getRemainingTime();
    }
};

// Event structure to manage events such as process arrival, CPU burst completion, etc.
struct Event {
    int time;
    string type;  // "ARRIVAL", "BURST_COMPLETE", "IO_COMPLETE", "PREEMPT"
    Process process;
};

// Helper function to print the current state of the ready queue
void printQueue(priority_queue<Process, vector<Process>, CompareProcess> readyQueue) {
    if (readyQueue.empty()) {
        cout << "empty]" << endl;
    } else {
        priority_queue<Process, vector<Process>, CompareProcess> tempQueue = readyQueue;
        while (!tempQueue.empty()) {
            cout << tempQueue.top().getId() << " ";
            tempQueue.pop();
        }
        cout << "]" << endl;
    }
}

// Function to simulate SRT scheduling
void simulateSRT(vector<Process> &processes, int contextSwitchTime, double alpha) {
    priority_queue<Process, vector<Process>, CompareProcess> readyQueue;
    vector<Event> events;  // List of events to process

    // Initialize events (e.g., process arrivals)
    for (auto& process : processes) {
        events.push_back({process.getArrivalTime(), "ARRIVAL", process});
    }

    // Sort events by time (initial event list)
    sort(events.begin(), events.end(), [](const Event& e1, const Event& e2) {
        return e1.time < e2.time;
    });

    int currentTime = 0;
    bool cpuIdle = true;
    Process currentProcess;

    while (!events.empty()) {
        Event event = events.front();
        events.erase(events.begin());  // Pop the event
        currentTime = event.time;

        if (event.type == "ARRIVAL") {
            cout << "time " << currentTime << "ms: Process " << event.process.getId()
                 << " (tau " << event.process.getTau() << "ms) arrived; added to ready queue [Q ";
            readyQueue.push(event.process);
            printQueue(readyQueue); 

            if (cpuIdle && !readyQueue.empty()) {
                currentProcess = readyQueue.top();
                readyQueue.pop();
                cpuIdle = false;
                cout << "time " << currentTime << "ms: Process " << currentProcess.getId()
                     << " started using the CPU for " << currentProcess.getFrontCPU() << "ms burst [Q ";
                printQueue(readyQueue);

                events.push_back({currentTime + currentProcess.getFrontCPU(), "BURST_COMPLETE", currentProcess});
            }
        } else if (event.type == "BURST_COMPLETE") {
            cout << "time " << currentTime << "ms: Process " << currentProcess.getId()
                 << " completed a CPU burst; " << currentProcess.getNumBursts() << " bursts to go [Q ";
            printQueue(readyQueue);

            // Calculate new tau
            currentProcess.popFrontCPU();
            currentProcess.recalculateTau(alpha); // Use the recalculateTau method from Process class

            if (!currentProcess.isEmptyIO()) {
                int ioCompleteTime = currentTime + currentProcess.getFrontIO();
                currentProcess.popFrontIO();
                events.push_back({ioCompleteTime, "IO_COMPLETE", currentProcess});
                cout << "time " << currentTime << "ms: Process " << currentProcess.getId()
                     << " switching out of CPU; blocking on I/O until time " << ioCompleteTime << "ms [Q ";
                printQueue(readyQueue);
            } else {
                cout << "time " << currentTime << "ms: Process " << currentProcess.getId() << " terminated [Q ";
                printQueue(readyQueue);
            }

            cpuIdle = true; // Mark CPU as idle
        } else if (event.type == "IO_COMPLETE") {
            cout << "time " << currentTime << "ms: Process " << event.process.getId()
                 << " completed I/O; added to ready queue [Q ";
            readyQueue.push(event.process);
            printQueue(readyQueue);

            if (cpuIdle && !readyQueue.empty()) {
                currentProcess = readyQueue.top();
                readyQueue.pop();
                cpuIdle = false;
                cout << "time " << currentTime << "ms: Process " << currentProcess.getId()
                     << " started using the CPU for " << currentProcess.getFrontCPU() << "ms burst [Q ";
                printQueue(readyQueue);

                events.push_back({currentTime + currentProcess.getFrontCPU(), "BURST_COMPLETE", currentProcess});
            }
        }

        sort(events.begin(), events.end(), [](const Event& e1, const Event& e2) {
            return e1.time < e2.time;
        });
    }

    cout << "time " << currentTime << "ms: Simulator ended for SRT [Q empty]" << endl;
}
*/