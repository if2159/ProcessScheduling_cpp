#include "Process.h"
#include <iostream>
using namespace std;


Process::Process(int st, int pid){
    startTime = st;
    PID = pid;
    tasks = new list<Task*>();
}

Process::Process(){
    startTime = -1;
    PID = -1;

}

void Process::addTask(Task* t){
        tasks->push_back(t);
}

int Process::getTaskListSize(){
    return tasks->size();
}

int Process::getStartTime() {
    return startTime;
}

 Task* Process::getNextTask(){
    if(!tasks->empty()){
        return (tasks->front());
    }
    return NULL;
}

 void Process::setLocation(ProcessLocation pl){
    location = pl;
}

 ProcessLocation Process::getLocation(){
    return location;
}


bool Process::update(){
    if(tasks->empty()){
        return true;
    }

    //cout<<"In Update\n";
    Task* currentTask = tasks->front();
    switch(currentTask->getType()){
        case TaskType::CORE:
            //cout<<"CORE*****************************\n";
            if(location == ProcessLocation::IN_CORE){
                bool b = currentTask->update();
                //std::cout<<"value of B" << b <<"\n";
                if(b){
                    
                    tasks->pop_front();
                    return true;
                }
                else{
                }
            }
           break;
        case TaskType::DISK:
            //cout<<"DISK*****************************\n";

            if (location == ProcessLocation::IN_DISK) {
                if (currentTask->update()) {
                    tasks->pop_front();
                    return true;
                }
            }
           break;
        case TaskType::DISPLAY:
            //cout<<"DISP*****************************\n";
            if(location == ProcessLocation::IN_DISPLAY){
                if(currentTask->update()){
                    tasks->pop_front();
                    return true;
                }
            }
           break;
        case TaskType::INPUT:
            //cout<<"INPUT*****************************\n";
            if (location == ProcessLocation::IN_DISPLAY) {
                if (currentTask->update()) {
                    tasks->pop_front();
                    return true;
                }
            }
           break;
        default:
            //cout<<"Broken*****************************\n";
            //Waiting in a queue. Do nothing.
           break;
    }
    return false;
}

bool Process::equals(Process &p){
    return (this->startTime == p.getStartTime() && this->PID == p.PID);
  }
bool Process::equals(Process* p){
    return (this->startTime == p->getStartTime() && this->PID == p->PID);
}
