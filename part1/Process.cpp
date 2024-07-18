#include "Process.h"

Process::Process()
    : pID_(""), arrivalTime_(0), numBursts_(0), cpuBound_(false), lambda_(0.0), upperBound_(0) {}

Process::Process(std::string pID, int arrivalTime, int numBursts, bool cpuBound, double lambda, int upperBound)
    : pID_(pID), arrivalTime_(arrivalTime), numBursts_(numBursts), cpuBound_(cpuBound), lambda_(lambda), upperBound_(upperBound) {}

//getters
std::string Process::getId() const {
    return pID_;
}

int Process::getArrivalTime() const {
    return arrivalTime_;
}

int Process::getNumBursts() const {
    return numBursts_;
}

bool Process::isCpuBound() const {
    return cpuBound_;
}

double Process::getLambda() const {
    return lambda_;
}

int Process::getUpperBound() const {
    return upperBound_;
}

//setters
void Process::setId(const std::string &pID) {
    pID_ = pID;
}

void Process::setArrivalTime(int arrivalTime) {
    arrivalTime_ = arrivalTime;
}

void Process::setNumBursts(int numBursts) {
    numBursts_ = numBursts;
}

void Process::setCpuBound(bool cpuBound) {
    cpuBound_ = cpuBound;
}

void Process::setLambda(double lambda) {
    lambda_ = lambda;
}

void Process::setUpperBound(int upperBound) {
    upperBound_ = upperBound;
}

void Process::setCpuBurstTime( int cpuBurstTime ) {
    cpuBurstTimes.push_back(cpuBurstTime);
}

void Process::setIOBurstTime( int ioBurstTime ) {
    IOBurstTimes.push_back(ioBurstTime);
}
