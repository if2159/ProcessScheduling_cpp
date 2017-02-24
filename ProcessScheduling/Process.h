#ifndef Process_H
#define Process_H
#include "ProcessLocation.h"
#include "Task.h"
#include <list>
class Process{
private:
    int startTime;
    std::list<Task> tasks;
    ProcessLocation location;
public:
    int PID;
    Process();
    Process(int st, int pid);
    void addTask(Task t);
    int getStartTime() const;
    Task getNextTask();
    void setLocation(ProcessLocation pl);
    ProcessLocation getLocation();
    bool update();  
    bool operator==(const Process &p) const;
    bool operator!=(const Process &p) const;
};

#endif