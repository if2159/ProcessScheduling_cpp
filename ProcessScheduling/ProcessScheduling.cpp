#include <iostream>
#include "Task.h"
#include "Process.h"
#include "Core.h"
#include "Disk.h"
#include <queue>

using namespace std;

Core *coreArray;
vector<Process*> processList;
Disk disk;
list<Process*> readyQueue;
list<Process*> diskQueue;
vector<Process*>  displayList;
vector<Process*> waitingList;
int tick = 0;
int workingTasks = 1;
int currentPID = 0;
int coreSize = 0;


void terminateProcess(Process* p) {
    //cout<<"TERMINATING!\n\tPID: "<<p->PID<<"\n";
    cout<< "\n\n";
    cout<<"CURRENT STATE OF THE SYSTEM AT t = ";
    cout<< tick;
    cout<< " ms:\n";
    int busyCores = 0;
    for(int i = 0; i < coreSize; i++){
        Core c = coreArray[i];
        if(!c.isAvailable()){
            busyCores++;
        }
    }
    cout<< "Current number of busy cores:" ;
    cout<<  busyCores;
    cout<<  "\n";
    cout<< "READY QUEUE:\n";
    bool readyQueueEmpty = true;
    vector<Process> vp(readyQueue.size());
    for(Process* pr: readyQueue){
        readyQueueEmpty = false;
        cout<< "Process: ";
        cout<<  pr->PID + "\n";
    }
    if(readyQueueEmpty){
        cout<< "Empty\n";
    }
    
    cout<< "DISK QUEUE:\n";
    bool diskQueueEmpty = true;
    for(Process* pr: diskQueue){
        diskQueueEmpty = false;
        cout<< "Process: " ;
        cout<<  pr->PID + "\n";
    }
    if(diskQueueEmpty){
        cout<< "Empty\n";
    }
    cout<< "PROCESS TABLE\n";
    for(Process* pr: processList){
        cout<< "Process ";
        cout<< pr->PID;
        cout<< " started at ";
        cout<< pr->getStartTime();
        cout<< " ms and is ";
        if(pr->getNextTask() == NULL){
            cout<< "TERMINATED\n";
        }
        else{
            switch(pr->getLocation()){
                case ProcessLocation::IN_CORE:
                    cout<< "RUNNING\n";
                    break;
                case  ProcessLocation::IN_DISK:
                    cout<< "BLOCKED - DISK\n";
                    break;
                case  ProcessLocation::IN_DISPLAY:
                    cout<< "BLOCKED - DISPLAY\n";
                    break;
                case ProcessLocation::DISK_QUEUE:
                case ProcessLocation::READY_QUEUE:
                case ProcessLocation::WAITING:
                    cout<< "READY\n";
                break;

            } 
       }
    }
    for(int i = 0; i < processList.size(); i++){
        if(processList[i]->equals(p)){
            processList.erase(processList.begin() + i);
        }
    }
    cout<< "\n\n\n\n";
    
}


 void handleInput(string s, int n){
    //cout<<s<<" " << n << "\n";
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
        Process* p = new Process(n,currentPID++);
        p->setLocation(ProcessLocation::WAITING);
        processList.push_back(p);
        waitingList.push_back(p);
    }
    else if(s == "CORE"){
        Task* t = new Task(TaskType::CORE,n);
        //cout<<"pew\n";
        //cout<<"Task Size:"<<waitingList.size()<<"\n";
        waitingList[(waitingList.size()-1)]->addTask(t);
    }
    else if(s == "DISK"){
        waitingList[(waitingList.size()-1)]->addTask(new Task(TaskType::DISK, n));
    }
    else if(s == "DISPLAY"){
        waitingList[(waitingList.size()-1)]->addTask(new Task(TaskType::DISPLAY, n));
    }
    else if(s == "INPUT"){
        waitingList[(waitingList.size()-1)]->addTask(new Task(TaskType::DISPLAY, n));
    }

}

void readFile(){
    //std::ifstream in("C:\\Users\\Ian Fennen\\Desktop\\School Work\\Spring 2017\\COSC 3360\\HW1\\c++\\ProcessScheduling_cpp\\ProcessScheduling\\input12.txt");
    //std::cin.rdbuf(in.rdbuf());
    string s = "";
    while(cin >> s){
        int n = -1;
        cin >> n;
        handleInput(s,n);
    }
}



bool stillWorking(){
    //cout<<"Still working \n";
    for(int i = 0; i < processList.size(); i++){
        Process* p = processList[i];
        //cout<<"Checking...\n";
        if(p->getNextTask()!= NULL){
            //cout<<"It has stuff!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
            return true;
        }
    }
    //cout<<"It's empty...\n";
    return false;
}

void emptyWaitList(){
    //Empty WaitList
/*    for (int i = 0; i < waitingList.size(); i++) {
        //cout<<"\t"<<waitingList[i]->getTaskListSize()<<"\n";

    }*/

    for (int i = 0; i < waitingList.size(); i++){
        Process* p = waitingList[i];
        if (p->getStartTime() <= tick) {
            if(p->getNextTask()!= NULL){
                switch (p->getNextTask()->getType()) {
                    case TaskType::CORE:
                        readyQueue.push_back(p);
                        break;
                    case TaskType::DISK:
                        diskQueue.push_back(p);
                        break;
                    case TaskType::DISPLAY:
                    case TaskType::INPUT:
                        p->setLocation(ProcessLocation::IN_DISPLAY);
                        displayList.push_back(p);
                        break;

                    case TaskType::INVALID:break;
                }
    //cout<<"erase\n";
        //waitingList.erase(waitingList.begin()+i);
                waitingList.erase(waitingList.begin()+i);
                i--;
    //cout<<"afterErase\n";

                workingTasks++;
            }
            else{
               // cout<<"SHOULD TERMINATE PID: "<<waitingList[i]->PID<<"\n";
                waitingList.erase(waitingList.begin()+i);
                terminateProcess(p);
                workingTasks--;
            }
        }
    }
}


 void changeCurrentTasks(){
    //cout<<"CORES\n";
     //cout<<"\t"<<readyQueue.front()->getTaskListSize()<<"\n";
     for(int i = 0; i < coreSize; i++){
        Core c = coreArray[i];
        //Task* tl = c.getCurrentProcess()->getNextTask();
        //cout<<"NULL TEST "<< tl->getTimeLeft()<<"\n";
        if(c.isAvailable() && !readyQueue.empty()){
            Process* p = readyQueue.front();
            readyQueue.pop_front();
            p->setLocation(ProcessLocation::IN_CORE);
            c.setCurrentProcess(p);
            coreArray[i] = c;
            //cout <<"NEW TASK IN CORE PID: " << p->PID << "\n";

        }
    }
    //cout<<"DISK\n";
    if (disk.isAvailable() && !diskQueue.empty()) {
        Process* p = diskQueue.front();
        diskQueue.pop_front();
        p->setLocation(ProcessLocation::IN_DISK);
        disk.setCurrentProcess(p);

    }
     //cout<<"AFTER DISK\n";
    
}


 void updateCore(){
    changeCurrentTasks();
     //cout<<"Update COre******\n";
    for(int i = 0; i < coreSize; i++){
        Core c = coreArray[i];
        Process* p = c.update();
        if(p != NULL){
//            cout<<"Process finished CORE PID: "<<p->PID<<"\n";

            p->setLocation(ProcessLocation::WAITING);
            c.setCurrentProcess(NULL);
            
            waitingList.push_back(p);
            workingTasks--;
        }
        coreArray[i] = c;
    }
}

 void updateDisplay(){
    for(int i = 0; i < displayList.size(); i++){
        Process* p = displayList[i];
        if(p->update()){
 //           cout<<"Process finished DISPLAY PID: "<<p->PID<<"\n";

            displayList.erase(displayList.begin()+i);
            waitingList.push_back(p);
            workingTasks--;
            i--;
        }
        
    }
}

 void updateDisk(){
    //DISK
    Process* p = disk.update();
    if (p != NULL) {
 //       cout<<"Process finished DISK PID: "<<p->PID<<"\n";
        p->setLocation(ProcessLocation::WAITING);
        
        waitingList.push_back(p);
        workingTasks--;
 //       cout<<"new task in DISK PID: "<<p->PID<<"\n";
    }
    

}





 
 void update(){
//    cout<<"1\n";
    changeCurrentTasks();
//    cout<<"2\n";
    emptyWaitList();
//    cout<<"3\n";
    changeCurrentTasks();
//    cout<<"4\n";
    updateDisk();
//    cout<<"5\n";
    updateDisplay();
//    cout<<"6\n";
    updateCore();
//    cout<<"7\n";
    emptyWaitList();
//    cout<<"8\n";
    changeCurrentTasks();        
}

int main() {
//    waitingList();
    //cout<<"starting\n";
    readFile();

    while(stillWorking() || !waitingList.empty()){
        update();
       // cout<<tick<<"\n";
        tick++;
    }
    return 0;
}

