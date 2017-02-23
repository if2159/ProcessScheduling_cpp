#ifndef Process_H
#define Process_H
#include "Process.h"
#include "ProcessLocation.h"
class Process{
private:
    const int startTime;
    List<Task> tasks;
    const int PID;
    ProcessLocation location;
public:
    Task(TaskType tt, int initialTime);
    TaskType getType();
    int getTimeLeft();
    bool update();
};

#endif