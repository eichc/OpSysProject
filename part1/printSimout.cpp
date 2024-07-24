#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

void printSimout(int p, int CPUp, int IOp, double CPUavgCPUBurst, double IOavgCPUBurst, double avgCPUBurst, double CPUavgIOBurst, double IOavgIOBurst, double avgIOBurst) {
    ofstream SimOut("simout.txt");
    SimOut << "-- number of processes: " << p << endl;
    SimOut << "-- number of CPU-bound processes: " << CPUp << endl;
    SimOut << "-- number of I/O-bound processes: " << IOp << endl;
    SimOut << "-- CPU-bound average CPU burst time: " << setprecision(4) << CPUavgCPUBurst <<" ms" << endl;
    SimOut << "-- IO-bound average CPU burst time: " << setprecision(4) << IOavgCPUBurst <<" ms" << endl;
    SimOut << "-- overall average CPU burst time: " << setprecision(4) << avgCPUBurst << " ms" << endl;
    SimOut << "-- CPU-bound average I/O burst time: " << setprecision(4) << CPUavgIOBurst << " ms" << endl;
    SimOut << "-- I/O-bound average I/O burst time: " << setprecision(4) << IOavgIOBurst << " ms" << endl;
    SimOut << "-- overall average I/O burst time: " << setprecision(4) << avgIOBurst << " ms" << endl;
    SimOut.close();
}