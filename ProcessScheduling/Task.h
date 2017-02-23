#ifndef Task_H
#define Task_H
#include "TaskType.h"
class Task{
private:
    TaskType type;
    int timeLeft;
public:
	Task();
    Task(TaskType tt, int initialTime);
    TaskType getType();
    int getTimeLeft();
    bool update();
};

#endif