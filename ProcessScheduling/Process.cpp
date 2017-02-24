#include "Process.h"
#include <iostream>

using namespace std;

Task NullTask(TaskType::INVALID,-1);

Process::Process(int st, int pid){
    startTime = st;
    PID = pid;
    //tasks();
}

Process::Process(){
    startTime = -1;
    PID = -1;

}

void Process::addTask(Task t){
        tasks.push_back(t);
}

int Process::getStartTime() {
    return startTime;
}

 Task Process::getNextTask(){
    if(!tasks.empty()){
        return tasks.front();
    }
    return NullTask;
}

 void Process::setLocation(ProcessLocation pl){
    location = pl;
}

 ProcessLocation Process::getLocation(){
    return location;
}


bool Process::update(){
    if(tasks.empty()){
        return true;
    }
    Task currentTask = tasks.front();
    switch(currentTask.getType()){
        case TaskType::CORE:
            if(location == ProcessLocation::IN_CORE){
                if(currentTask.update()){
                    
                    tasks.pop_front();
                    return true;
                }
                else{
                }
            }
           break;
        case TaskType::DISK:
            if (location == ProcessLocation::IN_DISK) {
                if (currentTask.update()) {
                    tasks.pop_front();
                    return true;
                }
            }
           break;
        case TaskType::DISPLAY:
            if(location == ProcessLocation::IN_DISPLAY){
                if(currentTask.update()){
                    tasks.pop_front();
                    return true;
                }
            }
           break;
        case TaskType::INPUT:
            if (location == ProcessLocation::IN_DISPLAY) {
                if (currentTask.update()) {
                    tasks.pop_front();
                    return true;
                }
            }
           break;
        default:
            //Waiting in a queue. Do nothing.
           break;
    }
    return false;
}

bool Process::equals(Process &p){
    return (this->startTime == p.getStartTime() && this->PID == p.PID);
  }
