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

    bool isCpuBound() const;
    void setCpuBound(bool cpuBound);

    void setCpuBurstTime( int cpuBurstTime );

    void setIOBurstTime( int ioBurstTime );
    
    void outputProcess();

private:
    std::string pID_;
    int arrivalTime_;
    int numBursts_;
    bool cpuBound_;

    std::vector<int>cpuBurstTimes;
    std::vector<int>IOBurstTimes;
};

#endif
