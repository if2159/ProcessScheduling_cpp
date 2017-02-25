#include "Core.h"
#include <iostream>
using namespace std;



Core::Core(int ts){
    timeSlice = ts;
    available = true;
    currentProcess = NULL;
}
Core::Core(){
    timeSlice = 0;
    available = true;
    currentProcess = NULL;
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

Process* Core::getCurrentProcess() {
    return currentProcess;
}

void Core::setCurrentProcess(Process* p){
    currentProcess = p;
    if(p!=NULL){
        available = false;
        sliceRemaining = timeSlice;
    }
}

Process* Core::update(){
    //cout<<"UPDATING IN THE CORE\n";
    if(currentProcess != NULL && sliceRemaining > 0){
        available = false;
        if(currentProcess->update()){
            available = true;
            return currentProcess;
        }
        sliceRemaining--;
        return NULL;
    }
    available = true;
    return currentProcess;
}

