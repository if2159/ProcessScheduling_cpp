#include "Disk.h"
#include <iostream>
using namespace std;


extern Process nullProcess;

 Disk::Disk(){
        timeRemaining = 0;
        available = true;
        
    }

 int Disk::getTimeRemaining() {
    return timeRemaining;
}

 bool Disk::isAvailable() {
    return available;
}

 Process Disk::getCurrentProcess() {
    return currentProcess;
}

 void Disk::setCurrentProcess(Process p){
    currentProcess = p;
    timeRemaining = p.getNextTask().getTimeLeft();
}

 Process Disk::update(){
    if(!currentProcess.equals(nullProcess)){
        available = false;
        if(currentProcess.update()){
            available = true; 
            Process p = currentProcess;
            currentProcess = nullProcess;
            return p;
        }
        timeRemaining--;
        return nullProcess;
    }
    Process p = currentProcess;
    available = true;
    currentProcess = nullProcess;
    return p;
}