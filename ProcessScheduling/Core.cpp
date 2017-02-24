#include "Core.h"
#include <iostream>
using namespace std;


extern Process nullProcess;

Core::Core(int ts){
    timeSlice = ts;
    available = true;
    currentProcess = nullProcess;
}
Core::Core(){
    timeSlice = 0;
    available = true;
    currentProcess = nullProcess;
}

int Core::getTimeSlice() {
    return timeSlice;
}

int Core::getSliceRemaining() {
    return sliceRemaining;
}

bool Core::isAvailable() {
    return available;
}

Process Core::getCurrentProcess() {
    return currentProcess;
}

void Core::setCurrentProcess(Process p){
    currentProcess = p;
    if(!p.equals(nullProcess)){
        available = false;
        sliceRemaining = timeSlice;
    }
}

Process Core::update(){
    if(!currentProcess.equals(nullProcess) && sliceRemaining > 0){
        available = false;
        if(currentProcess.update()){
            available = true;
            return currentProcess;
        }
        sliceRemaining--;
        return nullProcess;
    }
    available = true;
    return currentProcess;
}

