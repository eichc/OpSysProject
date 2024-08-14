#include <queue>
#include <vector>
#include "Process.h"
/*
struct CompareRemainingTime { //need to implement getting remaining cpu burst time
    bool operator()(const Process& p1, const Process& p2) {
        
        
        
        // return p1.getRemainingCpuBurstTime() > p2.getRemainingCpuBurstTime();
    }
};

void ShortestJobRemaining(std::vector<Process> processes) {
    std::priority_queue<Process, std::vector<Process>, CompareRemainingTime> SJFqueue;
    int currentTime = 0;
    int totalProcesses = processes.size();
    int completedProcesses = 0;

    // Initially, add all processes to the ready queue
    for (auto &process : processes) {
        SJFqueue.push(process);
    }

    // Process scheduling loop
    while (completedProcesses < totalProcesses) {
        if (!SJFqueue.empty()) {
            // Get the process with the shortest remaining time
            Process currentProcess = SJFqueue.top();
            SJFqueue.pop();

            // Calculate the time slice to execute (use full remaining burst time or a predefined slice)
            int timeSlice = currentProcess.getRemainingCpuBurstTime(); // Use entire remaining time

            // Execute the process for this time slice
            currentProcess.executeCpuBurst(timeSlice);
            currentTime += timeSlice;

            // Check if the process is finished
            if (currentProcess.getRemainingCpuBurstTime() == 0) {
                completedProcesses++;
                std::cout << "Process " << currentProcess.getId() << " completed at time " << currentTime << "ms\n";
            } else {
                // If not finished, put it back into the ready queue
                SJFqueue.push(currentProcess);
            }
        } else {
            // If the ready queue is empty, increase the current time to the arrival time of the next process
            if (completedProcesses < totalProcesses) {
                currentTime++;
            }
        }
    }

    std::cout << "All processes have been completed.\n";
}
*/