#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cmath>

using namespace std;

void printSimout(int p, int CPUp, int IOp, double CPUavgCPUBurst, double IOavgCPUBurst, double avgCPUBurst, double CPUavgIOBurst, double IOavgIOBurst, double avgIOBurst) {
    ofstream SimOut("simout.txt");
    SimOut << "-- number of processes: " << p << endl;
    SimOut << "-- number of CPU-bound processes: " << CPUp << endl;
    SimOut << "-- number of I/O-bound processes: " << IOp << endl;
    SimOut << "-- CPU-bound average CPU burst time: " << (ceil((CPUavgCPUBurst *1000))/1000) <<" ms" << endl;
    SimOut << "-- IO-bound average CPU burst time: " <<  (ceil((IOavgCPUBurst *1000))/1000) <<" ms" << endl;
    SimOut << "-- overall average CPU burst time: " << (ceil((avgCPUBurst *1000))/1000) << " ms" << endl;
    SimOut << "-- CPU-bound average I/O burst time: " << (ceil((CPUavgIOBurst *1000))/1000) << " ms" << endl;
    SimOut << "-- I/O-bound average I/O burst time: " << (ceil((IOavgIOBurst *1000))/1000) << " ms" << endl;
    SimOut << "-- overall average I/O burst time: " << (ceil((avgIOBurst *1000))/1000) << " ms" << endl;
    SimOut.close();
}