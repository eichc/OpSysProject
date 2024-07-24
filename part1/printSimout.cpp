#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cmath>

using namespace std;

void printSimout(int p, int CPUp, int IOp, double CPUavgCPUBurst, double IOavgCPUBurst, double avgCPUBurst, double CPUavgIOBurst, double IOavgIOBurst, double avgIOBurst) {
    FILE *fp;
    fp = fopen("simout.txt", "w");
    
    fprintf(fp, "-- number of processes: %d\n", p);
    fprintf(fp, "-- number of CPU-bound processes: %d\n", CPUp);
    fprintf(fp, "-- number of I/O-bound processes: %d\n", IOp);
    fprintf(fp, "-- CPU-bound average CPU burst time: %.3f ms\n", CPUavgCPUBurst);
    fprintf(fp, "-- I/O-bound average CPU burst time: %.3f ms\n", IOavgCPUBurst);
    fprintf(fp, "-- overall average CPU burst time: %.3f ms\n", avgCPUBurst);
    fprintf(fp, "-- CPU-bound average I/O burst time: %.3f ms\n", CPUavgIOBurst);
    fprintf(fp, "-- I/O-bound average I/O burst time: %.3f ms\n", IOavgIOBurst);
    fprintf(fp, "-- overall average I/O burst time: %.3f ms\n", avgIOBurst);

    fclose(fp);
}