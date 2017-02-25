#include "Disk.h"
#include <iostream>
using namespace std;



 Disk::Disk(){
        timeRemaining = 0;
        available = true;
        currentProcess = NULL;
    }

 int Disk::getTimeRemaining() {
    return timeRemaining;
}

 bool Disk::isAvailable() {
    return available;
}

 Process* Disk::getCurrentProcess() {
    return currentProcess;
}

 void Disk::setCurrentProcess(Process* p){
    currentProcess = p;
    timeRemaining = p->getNextTask()->getTimeLeft();
}

 Process* Disk::update(){
    if(currentProcess != NULL){
        available = false;
        if(currentProcess->update()){
            available = true; 
            Process* p = currentProcess;
            currentProcess = NULL;
            return p;
        }
        timeRemaining--;
        return NULL;
    }
    Process* p = currentProcess;
    available = true;
    currentProcess = NULL;
    return p;
}