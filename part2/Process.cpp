#include "Process.h"

Process::Process()
    : pID_(""), arrivalTime_(0), numBursts_(0), cpuBound_(false), blocking(-1), tau_(0), remainingTime_(0), inIO_(false) {}

Process::Process(std::string pID, int arrivalTime, int numBursts, bool cpuBound)
    : pID_(pID), arrivalTime_(arrivalTime), numBursts_(numBursts), cpuBound_(cpuBound), blocking(-1), tau_(0), remainingTime_(0), inIO_(false) {}

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
    std::cout << pID_ << ": arrival time " << arrivalTime_ << "ms; " << numBursts_ << " CPU bursts\n";
}

void Process::setBlocking(int time) {
    blocking = time;
}

int Process::getBlocking() const {
    return blocking;
}

<<<<<<< Updated upstream
// Tau methods
=======
// Tau-related methods
>>>>>>> Stashed changes
int Process::getTau() const {
    return tau_;
}

<<<<<<< Updated upstream
void Process::setTau(int newTau) {
    tau_ = newTau;
=======
void Process::setTau(int t) {
    tau_ = t;
}

void Process::recalculateTau(double alpha) {
    int oldTau = tau_;
    tau_ = ceil(alpha * getFrontCPU() + (1 - alpha) * tau_);
    std::cout << "Recalculated tau for process " << pID_ << ": old tau " << oldTau << "ms ==> new tau " << tau_ << "ms\n";
}

void Process::setRemainingTime(int time) {
    remainingTime_ = time;
}

int Process::getRemainingTime() const {
    return remainingTime_;
}

void Process::startIO(int currentTime) {
    inIO_ = true;
    blocking = currentTime + popFrontIO(); // Example I/O time handling
    std::cout << "Process " << pID_ << " switching out of CPU; blocking on I/O until time " << blocking << "ms\n";
}

bool Process::isComplete() const {
    return cpuBurstTimes.empty();
}

void Process::completeIO(int currentTime) {
    inIO_ = false;
    arrivalTime_ = currentTime;
    std::cout << "Process " << pID_ << " completed I/O; added to ready queue\n";
}

bool Process::isInIO() const {
    return inIO_;
>>>>>>> Stashed changes
}

bool operator<(const Process& lhs, const Process& rhs) {
    if (lhs.getRemainingTime() == rhs.getRemainingTime()) {
        return lhs.getId().compare(rhs.getId());
    }
<<<<<<< Updated upstream
    return lhs.getFrontCPU() < rhs.getFrontCPU();
=======
    return lhs.getRemainingTime() < rhs.getRemainingTime();
>>>>>>> Stashed changes
}
