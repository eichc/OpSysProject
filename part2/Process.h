#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h>

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


    // part 2
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

    void setCpuBurstTime( int cpuBurstTime );

    void setIOBurstTime( int ioBurstTime );
    
    void outputProcess();

    void setBlocking(int time);
    int getBlocking() const;

    // Tau methods
    int getTau() const;
    void setTau(int newTau);

    void setRemaining(int time);
    int getRemaining() const;

    void setWaiting(int time);
    int getWaiting() const;

    void setStart(int time);
    int getStart() const;

    void setPredictedRemaining(int time);
    int getPredictedRemaining() const;

    void setTimeAdded(int time);
    int getTimeAdded() const;

private:
    std::string pID_;
    int arrivalTime_;
    int numBursts_;
    bool cpuBound_;
    int blocking; //time until released by IO
    int remaining; //remaining time if preempted
    int waiting; //time entered into waiting queue
    int burstStart;
    int predictedRemaining;
    int timeAdded;

    int tau_;  // New tau variable

    std::vector<int> cpuBurstTimes;
    std::vector<int> IOBurstTimes;
};

bool operator<(const Process& lhs, const Process& rhs);

#endif
