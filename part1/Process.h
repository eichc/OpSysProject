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
    Process(std::string pID, int arrivalTime, int numBursts, bool cpuBound, double lambda, int upperBound);

    std::string getId() const;
    void setId(const std::string &pID);

    int getArrivalTime() const;
    void setArrivalTime(int arrivalTime);

    int getNumBursts() const;
    void setNumBursts(int numBursts);

    bool isCpuBound() const;
    void setCpuBound(bool cpuBound);

    double getLambda() const;
    void setLambda(double lambda);

    int getUpperBound() const;
    void setUpperBound(int upperBound);

    void setCpuBurstTime( int cpuBurstTime );

    void setIOBurstTime( int ioBurstTime );
    
    void outputProcess();

private:
    std::string pID_;
    int arrivalTime_;
    int numBursts_;
    bool cpuBound_;
    double lambda_;
    int upperBound_;

    std::vector<int>cpuBurstTimes;
    std::vector<int>IOBurstTimes;
};

#endif
