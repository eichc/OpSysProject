#include "Process.h"

Process::Process()
    : pID_(""), arrivalTime_(0), numBursts_(0), cpuBound_(false), blocking(-1), remaining(-1), waiting(-1) {}

Process::Process(std::string pID, int arrivalTime, int numBursts, bool cpuBound)
    : pID_(pID), arrivalTime_(arrivalTime), numBursts_(numBursts), cpuBound_(cpuBound), blocking(-1), remaining(-1), waiting(-1) {}

// Getters
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

//for p2
int Process::getNumCPU() const {
    return cpuBurstTimes.size();
}

int Process::getFrontCPU() const {
    if (cpuBurstTimes.empty()) {
        return -1;
    }
    return cpuBurstTimes[0];
}

int Process::getFrontIO() const {
    return IOBurstTimes[0];
}

bool Process::isEmptyCPU() {
    return cpuBurstTimes.size() == 0;
}

bool Process::isEmptyIO() {
    return IOBurstTimes.size() == 0;
}

int Process::popFrontCPU() {
    if (!cpuBurstTimes.empty()) {
        int front = cpuBurstTimes.front();
        cpuBurstTimes.erase(cpuBurstTimes.begin());
        return front;
    }
    return -1;
}

int Process::popFrontIO() {
    if (!IOBurstTimes.empty()) {
        int front = IOBurstTimes.front();
        IOBurstTimes.erase(IOBurstTimes.begin());
        return front;
    }
    return -1;
}

void Process::pushFrontCPU(int cpuBurstTime) {
    cpuBurstTimes.insert(cpuBurstTimes.begin(), cpuBurstTime);
}

void Process::pushFrontIO(int ioBurstTime) {
    IOBurstTimes.insert(IOBurstTimes.begin(), ioBurstTime);
}

// Setters
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

void Process::setCpuBurstTime(int cpuBurstTime) {
    cpuBurstTimes.push_back(cpuBurstTime);
}

void Process::setIOBurstTime(int ioBurstTime) {
    IOBurstTimes.push_back(ioBurstTime);
}

void Process::outputProcess() {
    if (cpuBound_) {
        std::cout << "CPU-bound process ";
    } else {
        std::cout << "I/O-bound process ";
    }
    std::cout <<pID_<<": arrival time "<<arrivalTime_<<"ms; "<<numBursts_ << " CPU bursts\n";
}

void Process::setBlocking(int time) {
    blocking = time;
}

int Process::getBlocking() const {
    return blocking;
}

// Tau methods
int Process::getTau() const {
    return tau_;
}

void Process::setTau(int newTau) {
    tau_ = newTau;
}

void Process::setRemaining(int time) {
    remaining = time;
}

int Process::getRemaining() const {
    return remaining;
}

void Process::setWaiting(int time) {
    waiting = time;
}

int Process::getWaiting() const {
    return waiting;
}

bool operator<(const Process& lhs, const Process& rhs) {
    if (lhs.getFrontCPU() == rhs.getFrontCPU()) {
        return lhs.getId().compare(rhs.getId());
    }
    return lhs.getFrontCPU() < rhs.getFrontCPU();
}
