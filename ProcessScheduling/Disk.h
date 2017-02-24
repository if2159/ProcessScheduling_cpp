#ifndef Disk_H
#define Disk_H
#include "Process.h"
class Disk{
private:
    int timeRemaining;
    bool available;
    Process currentProcess;
public:
    Disk();
    Process getCurrentProcess(); 
    int     getTimeRemaining(); 
    bool isAvailable();
    void    setCurrentProcess(Process p);
    Process update();
};

#endif