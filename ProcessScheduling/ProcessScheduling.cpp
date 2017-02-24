#include <iostream>
#include "Task.h"
#include "Process.h"
#include "Core.h"
#include "Disk.h"
#include <list>
#include <vector>
using namespace std;

Core *coreArray;
vector<Process> processList;
Disk disk;
list<Process> readyQueue;
list<Process> diskQueue;
vector<Process>  displayList;
vector<Process> waitingList;
int tick = 0;
int workingTasks = 1;
int currentPID = 0;
Process nullProcess(-1, -1);
int coreSize = 0;
Task nullTask(TaskType::INVALID,-1);


void terminateProcess(Process p) {
    string sb = "";
    sb+= "\n\n";
    sb += "CURRENT STATE OF THE SYSTEM AT t = ";
    sb+= tick;
    sb+= " ms:\n";
    int busyCores = 0;
    for(int i = 0; i < coreSize; i++){
        Core c = coreArray[i];
        if(!c.isAvailable()){
            busyCores++;
        }
    }
    sb+= "Current number of busy cores:" ;
    sb+=  busyCores; 
    sb+=  "\n";
    sb+= "READY QUEUE:\n";
    bool readyQueueEmpty = true;
    for(const Process& pr: readyQueue){
        readyQueueEmpty = false;
        sb+= "Process: "; 
        sb+=  pr.PID + "\n";
    }
    if(readyQueueEmpty){
        sb+= "Empty\n";
    }
    
    sb+= "DISK QUEUE:\n";
    bool diskQueueEmpty = true;
    for(Process pr: diskQueue){
        diskQueueEmpty = false;
        sb+= "Process: " ;
        sb+=  pr.PID + "\n";
    }
    if(diskQueueEmpty){
        sb+= "Empty\n";
    }
    sb+= "PROCESS TABLE\n";
    for(Process pr: processList){
        sb+= "Process ";
        sb+= pr.PID;
        sb+= " started at ";
        sb+= pr.getStartTime();
        sb+= " ms and is ";
        if(pr.getNextTask().equals(nullTask)){
            sb+= "TERMINATED\n";
        }
        else{
            switch(pr.getLocation()){
                case ProcessLocation::IN_CORE:
                    sb+= "RUNNING\n";
                    break;
                case  ProcessLocation::IN_DISK:
                    sb+= "BLOCKED - DISK\n";
                    break;
                case  ProcessLocation::IN_DISPLAY:
                    sb+= "BLOCKED - DISPLAY\n";
                    break;
                case ProcessLocation::DISK_QUEUE:
                case ProcessLocation::READY_QUEUE:
                case ProcessLocation::WAITING:
                    sb+= "READY\n";
                break;
            } 
       }
    }
    for(int i = 0; i < processList.size(); i++){
        if(processList[i].equals(p)){
            processList.erase(processList.begin() + i);
        }
    }
    sb+= "\n\n\n\n";
    cout<<sb;
    
}


 void handleInput(string s, int n){
    cout<<s<<" " << n << "\n";
    if(s == "NCORES"){
            coreArray = new Core[n];
            coreSize = n;
    }
    else if(s == "SLICE"){
        for(int i = 0; i < coreSize; i++){
            coreArray[i] = Core(n);
        }
    }   
    else if(s == "NEW"){
        Process p = Process(n,currentPID++);
        p.setLocation(ProcessLocation::WAITING);
        processList.push_back(p);
        waitingList.push_back(p);
    }
    else if(s == "CORE"){
        waitingList[(waitingList.size()-1)].addTask(Task(TaskType::CORE,n));
    }
    else if(s == "DISK"){
        waitingList[(waitingList.size()-1)].addTask(Task(TaskType::DISK, n));
    }
    else if(s == "DISPLAY"){
        waitingList[(waitingList.size()-1)].addTask(Task(TaskType::DISPLAY, n));
    }
    else if(s == "INPUT"){
        waitingList[(waitingList.size()-1)].addTask(Task(TaskType::DISPLAY, n));
    }

}

void readFile(){
    string s = "";
    while(cin >> s){
        int n = -1;
        cin >> n;
        handleInput(s,n);
    }
}



bool stillWorking(){
    cout<<"Still working \n";
    for(Process p: processList){
        if(!p.getNextTask().equals(nullTask)){
            return true;
        }
    }
    return false;
}

void emptyWaitList(){
    //Empty WaitList
    for (int i = 0; i < waitingList.size(); i++){
        cout<<"INSIDE \n";
        Process p = waitingList[i];
        cout<<"getStartTime\n";

        if (p.getStartTime() <= tick) {
    cout<<"getNextTask\n";

            if(!p.getNextTask().equals(nullTask)){
    cout<<"switch\n";

                switch (p.getNextTask().getType()) {
                    case TaskType::CORE:
                        readyQueue.push_back(p);
                        break;
                    case TaskType::DISK:
                        diskQueue.push_back(p);
                        break;
                    case TaskType::DISPLAY:
                    case TaskType::INPUT:
                        p.setLocation(ProcessLocation::IN_DISPLAY);
                        displayList.push_back(p);
                        break;

                }
                i--;
    cout<<"erase\n";
        waitingList.erase(std::remove(waitingList.begin(), waitingList.end(), i), waitingList.end());
    cout<<"afterErase\n";

                workingTasks++;
            }
            else{
                waitingList.erase(waitingList.begin()+i);
                terminateProcess(p);
                workingTasks--;
            }
        }
    }
}


 void changeCurrentTasks(){
    cout<<"CORES\n";
    for(int i = 0; i < coreSize; i++){
        Core c = coreArray[i];
        if(c.isAvailable() && !readyQueue.empty()){
            Process p = readyQueue.front();
            readyQueue.pop_front();
            p.setLocation(ProcessLocation::IN_CORE);
            c.setCurrentProcess(p);
        }
    }
    cout<<"DISK\n";
    if (disk.isAvailable() && !diskQueue.empty()) {
        Process p = diskQueue.front();
        diskQueue.pop_front();
        p.setLocation(ProcessLocation::IN_DISK);
        disk.setCurrentProcess(p);

    }
    
}


 void updateCore(){
    changeCurrentTasks();
    for(int i = 0; i < coreSize; i++){
        Core c = coreArray[i];
        Process p = c.update();
        if(!p.equals(nullProcess)){
            p.setLocation(ProcessLocation::WAITING);
            c.setCurrentProcess(nullProcess);
            
            waitingList.push_back(p);
            workingTasks--;
        }
    }
}

 void updateDisplay(){
    for(int i = 0; i < displayList.size(); i++){
        Process p = displayList[i];
        if(p.update()){
            
            displayList.erase(displayList.begin()+i);
            waitingList.push_back(p);
            workingTasks--;
            i--;
        }
        
    }
}

 void updateDisk(){
    //DISK
    Process p = disk.update();
    if (!p.equals(nullProcess)) {
        p.setLocation(ProcessLocation::WAITING);
        
        waitingList.push_back(p);
        workingTasks--;
    }
    

}





 
 void update(){
    cout<<"1\n";
    changeCurrentTasks();
    cout<<"2\n";
    emptyWaitList();
    cout<<"3\n";
    changeCurrentTasks();
    cout<<"4\n";
    updateDisk();
    cout<<"5\n";
    updateDisplay();
    cout<<"6\n";
    updateCore();
    cout<<"7\n";
    emptyWaitList();
    cout<<"8\n";
    changeCurrentTasks();        
}

int main() {
    waitingList();
    readFile();

    while(stillWorking() || !waitingList.empty()){
        update();
        tick++;
    }
    return 0;
}

