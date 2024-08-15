#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <cmath>

class Process {
public:
    Process();
    Process(std::string pID, int arrivalTime, int numBursts, bool cpuBound);

    std::string getId() const;
    void setId(const std::string &pID);

    int getArrivalTime() const;
    void setArrivalTime(int arrivalTime);

    int getNumBursts() const;
    void setNumBursts(int numBursts);

    int getNumCPU() const;
    int getFrontCPU() const;
    int getFrontIO() const;

    bool isEmptyCPU();
    bool isEmptyIO();

    int popFrontCPU();
    int popFrontIO();

    void pushFrontCPU(int cpuBurstTime);
    void pushFrontIO(int ioBurstTime);

    bool isCpuBound() const;
    void setCpuBound(bool cpuBound);

    void setCpuBurstTime(int cpuBurstTime);
    void setIOBurstTime(int ioBurstTime);

    void outputProcess();

    void setBlocking(int time);
    int getBlocking() const;
    bool isComplete() const;

    // Tau-related methods
    void setTau(int t);
    int getTau() const;
    void recalculateTau(double alpha);

    void setRemainingTime(int time);
    int getRemainingTime() const;

    // I/O handling
    void startIO(int currentTime);
    void completeIO(int currentTime);
    bool isInIO() const;

    // Tau methods
    int getTau() const;
    void setTau(int newTau);

private:
    std::string pID_;
    int arrivalTime_;
    int numBursts_;
    bool cpuBound_;
    int blocking;

<<<<<<< Updated upstream
    int tau_;  // New tau variable
=======
    int tau_;
    int remainingTime_;
    bool inIO_;
>>>>>>> Stashed changes

    std::vector<int> cpuBurstTimes;
    std::vector<int> IOBurstTimes;
};

bool operator<(const Process& lhs, const Process& rhs);

#endif
