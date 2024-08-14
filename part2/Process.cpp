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

//for p2
int Process::getFrontCPU() const {
    return cpuBurstTimes[0];
}

int Process::getFrontIO() const {
    return IOBurstTimes[0];
}


bool Process::isEmptyCPU() { //check if this is correct ADD TO THE H FILE
    return cpuBurstTimes.size() == 0;
}

bool Process::isEmptyIO() { //check if this is correct ADD TO THE H FILE
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
    if (cpuBound_) {
        std::cout<<"CPU-bound process ";
    } else {
        std::cout<<"I/O-bound process ";
    }
    std::cout <<pID_<<": arrival time "<<arrivalTime_<<"ms; "<<numBursts_;
    if (numBursts_ == 1) {
        std::cout << " CPU burst:"<<std::endl;
    } else {
        std::cout << " CPU bursts:"<<std::endl;
    }
    int n = cpuBurstTimes.size();
    for (int i = 0; i < n; i++) {
        if (i == n-1) {
            std::cout<<"==> CPU burst "<<cpuBurstTimes[i]<<"ms"<<std::endl;
        }
        else {
            std::cout<<"==> CPU burst "<<cpuBurstTimes[i]<<"ms ==> I/O burst "<<IOBurstTimes[i]<<"ms"<<std::endl;

        }
    }
}

void Process::setBlocking(int time) {
    blocking = time;
}

int Process::getBlocking() const {
    return blocking;
}

bool operator<(const Process& lhs, const Process& rhs) {
    return lhs.getFrontCPU() < rhs.getFrontCPU();
}