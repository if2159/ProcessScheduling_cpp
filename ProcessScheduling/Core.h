#ifndef Core_H
#define Core_H
#include "Process.h"
class Core{
private:
    int sliceRemaining;
    bool available;
    Process currentProcess;
public:
    int timeSlice;
    Core(int ts);
    Process getCurrentProcess();
    int getSliceRemaining();
    int getTimeSlice();
    bool isAvailable(); 
    void setCurrentProcess(Process p);
    Process update();
};

#endif