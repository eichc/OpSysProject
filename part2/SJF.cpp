#include <queue>
#include <vector>
#include <algorithm>
#include "Process.h"

void ShortestJobRemaining(std::vector<Process> processes) {
    std::priority_queue<Process> SJFqueue;
    int currentTime = 0;
    int totalProcesses = processes.size();
    int completedProcesses = 0;

    std::cout << "time 0ms: Simulator started for SJF [Q empty]" << std::endl;

    // Sort processes by arrival time for correct processing
    std::sort(processes.begin(), processes.end(), [](const Process &p1, const Process &p2) {
        return p1.getArrivalTime() < p2.getArrivalTime();
    });

    size_t index = 0;

    while (completedProcesses < totalProcesses) {
        // Add processes that have arrived by current time to the ready queue
        while (index < processes.size() && processes[index].getArrivalTime() <= currentTime) {
            SJFqueue.push(processes[index]);
            std::cout << "time " << currentTime << "ms: Process " << processes[index].getId() << " (tau "<< processes[index].getFrontCPU() << "ms) arrived; added to ready queue [Q " << processes[index].getId() << "]" << std::endl;
            index++;
        }

        if (!SJFqueue.empty()) {
            // Get the process with the shortest CPU burst time
            Process currentProcess = SJFqueue.top();
            SJFqueue.pop();

            // Log the start of CPU burst
            std::cout << "time " << currentTime << "ms: Process " << currentProcess.getId() << " (tau " << currentProcess.getFrontCPU() << "ms) started using the CPU for "<< currentProcess.getFrontCPU() << "ms burst [Q empty]" << std::endl;


            // Execute the CPU burst
            int burstTime = currentProcess.getFrontCPU(); // Get the current burst time
            currentProcess.popFrontCPU(); // Remove the executed burst
            currentTime += burstTime;

            // Log CPU burst completion and recalculation of tau
            std::cout << "time " << currentTime << "ms: Process " << currentProcess.getId() << " completed a CPU burst; " << currentProcess.getNumBursts() - 1 << " bursts to go [Q empty]" << std::endl;
            int oldTau = currentProcess.getFrontCPU();
            int newTau = (oldTau + burstTime) / 2; // Simulate recalculation
            std::cout << "time " << currentTime << "ms: Recalculated tau for process " << currentProcess.getId() << ": old tau " << oldTau << "ms ==> new tau "<< newTau << "ms [Q empty]" << std::endl;

            // Log switching to IO
            burstTime = currentProcess.getFrontIO();
            currentProcess.popFrontIO();
            std::cout << "time " << currentTime << "ms: Process " << currentProcess.getId()<< " switching out of CPU; blocking on I/O until time "<< currentTime + burstTime << "ms [Q empty]" << std::endl;

            currentTime += burstTime; // Add IO burst time to current time

            // Check if the process is finished (all bursts are done)
            if (currentProcess.isEmptyCPU()) {
                completedProcesses++;
                std::cout << "Process " << currentProcess.getId() << " completed at time " << currentTime << "ms\n";
            } else {
                // Process should be re-queued after IO, but since it's non-preemptive, it waits for IO completion.
                SJFqueue.push(currentProcess);
            }
        } else {
            // If the ready queue is empty, advance the time to the next process's arrival time
            if (index < processes.size()) {
                currentTime = processes[index].getArrivalTime();
            }
        }
    }

    std::cout << "time " << currentTime << "ms: Simulator ended for SJF [Q empty]" << std::endl;
}
