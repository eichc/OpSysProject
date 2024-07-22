#include "Process.h"

Process::Process()
    : pID_(""), arrivalTime_(0), numBursts_(0), cpuBound_(false) {}

Process::Process(std::string pID, int arrivalTime, int numBursts, bool cpuBound)
    : pID_(pID), arrivalTime_(arrivalTime), numBursts_(numBursts), cpuBound_(cpuBound) {}

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

void Process::setCpuBurstTime( int cpuBurstTime ) {
    cpuBurstTimes.push_back(cpuBurstTime);
}

void Process::setIOBurstTime( int ioBurstTime ) {
    IOBurstTimes.push_back(ioBurstTime);
}

void Process::outputProcess() {
    std::cout<<"CPU-bound process "<<pID_<<": arrival time "<<arrivalTime_<<"ms; "<<numBursts_<<" CPU bursts:"<<std::endl;
    for (int i = 0; i < cpuBurstTimes.size(); i++) {
        if (i == IOBurstTimes.size()) {
            std::cout<<"==> CPU burst "<<cpuBurstTimes[i]<<"ms"<<std::endl;
        }
        else {
            std::cout<<"==> CPU burst "<<cpuBurstTimes[i]<<"ms ==> I/O burst "<<IOBurstTimes[i]<<"ms"<<std::endl;

        }
    }
}