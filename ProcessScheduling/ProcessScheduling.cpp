#include <iostream>
#include "Task.h"
#include "Process.h"
#include "Core.h"
#include "Disk.h"
#include <list>
using namespace std;

Core []coreArray;
std::vector<Process> processList;
Disk disk;
list<Process> readyQueue;
list<Process> diskQueue;
vector<Process>  displayList;
vector<Process> waitingList;
int tick = 0;
int workingTasks = 1;
int currentPID = 0;

/**
 * @param args the command line arguments
 */
int main() {
/*    readyQueue = new LinkedList();
    diskQueue  = new LinkedList();
    waitingList = new ArrayList();
    displayList = new ArrayList();
    processList = new ArrayList();
    */
    try{
        readFile();
    }
    catch(FileNotFoundException e){
        out.println("File  Not Found.");
    }
    out.println("*************START*************");
    while(stillWorking()|| !waitingList.isEmpty()){
        update();
        
        
        
        tick++;
        if(tick > 99 && tick %100 == 0){
            //outputState();
        }
    }
    
}

boolean stillWorking(){
    for(Process p: processList){
        if(p.getNextTask() != null){
            return true;
        }
    }
    return false;
}

void emptyWaitList(){
    //Empty WaitList
    for (int i = 0; i < waitingList.size(); i++){
        Process p = waitingList.get(i);
        //out.println(p);
        if (p.getStartTime() <= tick) {
            if(p.getNextTask() != null){
                //out.println("TICK: " + tick);
                switch (p.getNextTask().getType()) {
                    case CORE:
                        if(p.PID == 0)out.println("CORE PID:" + p.PID);
                        readyQueue.add(p);
                        break;
                    case DISK:
                        if(p.PID == 0)
                        out.println("DISK PID:" + p.PID);
                        diskQueue.add(p);
                        break;
                    case DISPLAY:
                    case INPUT:
                        //out.println("INPUT PID:" + p.PID);
                        if(p.PID == 0)
                        out.println("BEGIN DISPLAY for PID: " + p.PID + "at TIME: " + tick);
                        p.setLocation(ProcessLocation.DISPLAY);
                        displayList.add(p);
                        break;

                }
                i--;
                waitingList.remove(p);
                workingTasks++;
            }
            else{
                waitingList.remove(p);
                terminateProcess(p);
                workingTasks--;
            }
        }
    }
    return 0;
}


void update(){
    updateDisk();
    updateDisplay();
    updateCore();
    changeCurrentTasks();
    emptyWaitList();
    changeCurrentTasks();
    
    
    emptyWaitList();
    changeCurrentTasks();
    out.println(tick);
    
}

void updateCore(){
    changeCurrentTasks();
    for (Core c : coreArray) {
        Process p = c.update();
        if(p != null){
            p.setLocation(ProcessLocation.WAITING);
            c.setCurrentProcess(null);
            if(p.PID == 0)
            out.println("CORE completion for process: " + p.PID + " at time: " + tick);
            waitingList.add(p);
            workingTasks--;
        }
    }
}

void updateDisplay(){
    for(int i = 0; i < displayList.size(); i++){
        Process p = displayList.get(i);
        if(p.update()){
            if(p.PID == 0)
            out.println("DISPLAY completion for process: " + p.PID + " at time: " + tick);
            displayList.remove(p);
            waitingList.add(p);
            workingTasks--;
            i--;
        }
        
    }
}

void updateDisk(){
    //DISK
    Process p = disk.update();
    if (p != null) {
        p.setLocation(ProcessLocation.WAITING);
        if(p.PID == 0)
        out.println("DISK completion for process: " + p.PID + " at time: " + tick);
        waitingList.add(p);
        workingTasks--;
    }
    if(disk.isAvailable() && !diskQueue.isEmpty()){
        p = diskQueue.pop();
if(p.PID == 0)
        out.println("Process: " + p.PID + "got DISK at: " + tick + "for " + p.getNextTask().getTimeLeft());
        p.setLocation(ProcessLocation.DISK);
        disk.setCurrentProcess(p);
        
    }

}

void changeCurrentTasks(){
    for(Core c: coreArray){
        if(c.isAvailable() && !readyQueue.isEmpty()){
            Process p = readyQueue.pop();
            if(p.PID == 0)out.println("Process " + p.PID + " got CORE at: " + tick);
            p.setLocation(ProcessLocation.CORE);
            c.setCurrentProcess(p);
        }
    }
    
}
//TODO Something is causing a blocked state or the processes in a core are set to waiting state. Unsure of whats happening
void readFile() throws FileNotFoundException{
    out.println("Number:");
    //byte selection = (new Scanner(System.in)).nextByte();
    Scanner fileIn;
    /*switch(selection){
        case 0:
            fileIn = new Scanner(new File("input10.txt"));
            break;
        case 1:
            fileIn = new Scanner(new File("input11.txt"));
            break;
        default:
            throw new FileNotFoundException("File number " + selection + " not found");
    }*/
    fileIn = new Scanner(new File("input11.txt"));
    disk = new Disk();
    while(fileIn.hasNext()){
        String s = fileIn.next();
        int n    = fileIn.nextInt();
        handleInput(s,n);
    }
}

void handleInput(String s, int n){
    out.println("new INPUT: " + s + " " + n);
    switch(s){
        case "NCORES":
            coreArray = new Core[n];
            break;
        case "SLICE":
            for(int i = 0; i < coreArray.length; i++){
                coreArray[i] = new Core(n);
            }
            break;
        case "NEW":
            out.println("NEW" + n);
            Process p = new Process(n,currentPID++);
            p.setLocation(ProcessLocation.WAITING);
            processList.add(p);
            waitingList.add(p);
            break;
        case "CORE":
            waitingList.get(waitingList.size()-1).addTask(new Task(TaskType.CORE,n));
            break;
        case "DISK":
            waitingList.get(waitingList.size()-1).addTask(new Task(TaskType.DISK, n));
            break;
        case "DISPLAY":
            waitingList.get(waitingList.size()-1).addTask(new Task(TaskType.DISPLAY, n));
            break;
        case "INPUT":
            waitingList.get(waitingList.size()-1).addTask(new Task(TaskType.DISPLAY, n));
            break;
    
    
    
    }
}

void terminateProcess(Process p) {
    StringBuilder sb = new StringBuilder();
    sb.append("\n\n");
    sb.append("CURRENT STATE OF THE SYSTEM AT t = " + tick + " ms:\n");
    int busyCores = 0;
    for(Core c: coreArray){
        if(!c.isAvailable()){
            busyCores++;
        }
    }
    sb.append("Current number of busy cores:" +  busyCores + "\n");
    sb.append("READY QUEUE:\n");
    boolean readyQueueEmpty = true;
    for(Process pr: readyQueue){
        readyQueueEmpty = false;
        sb.append("Process: " + pr.PID + "\n");
    }
    if(readyQueueEmpty){
        sb.append("Empty\n");
    }
    
    sb.append("DISK QUEUE:\n");
    boolean diskQueueEmpty = true;
    for(Process pr: diskQueue){
        diskQueueEmpty = false;
        sb.append("Process: " + pr.PID + "\n");
    }
    if(diskQueueEmpty){
        sb.append("Empty\n");
    }
    sb.append("PROCESS TABLE\n");
    for(Process pr: processList){
        sb.append("Process " + pr.PID + " started at " + pr.getStartTime() + " ms and is ");
        if(pr.getNextTask() == null){
            sb.append("TERMINATED\n");
        }
        else{
            switch(pr.getLocation()){
                case CORE:
                    sb.append("RUNNING\n");
                    break;
                case DISK:
                    sb.append("BLOCKED - DISK\n");
                    break;
                case DISPLAY:
                    sb.append("BLOCKED - DISPLAY\n");
                    break;
                case DISK_QUEUE:
                case READY_QUEUE:
                case WAITING:
                    sb.append("READY\n");
                break;
            } 
       }
    }
    processList.remove(p);
    sb.append("\n\n\n\n");
    out.println(sb.toString());
    
}

void outputState(){
    StringBuilder sb = new StringBuilder();
    sb.append("\n\n");
    sb.append("CURRENT STATE OF THE SYSTEM AT t = " + tick + " ms:\n");
    int busyCores = 0;
    for(Core c: coreArray){
        if(!c.isAvailable()){
            busyCores++;
        }
    }
    sb.append("Current number of busy cores:" +  busyCores + "\n");
    sb.append("READY QUEUE:\n");
    boolean readyQueueEmpty = true;
    for(Process pr: readyQueue){
        readyQueueEmpty = false;
        sb.append("Process: " + pr.PID + "\n");
    }
    if(readyQueueEmpty){
        sb.append("Empty\n");
    }
    
    sb.append("DISK QUEUE:\n");
    boolean diskQueueEmpty = true;
    for(Process pr: diskQueue){
        diskQueueEmpty = false;
        sb.append("Process: " + pr.PID + "\n");
    }
    if(diskQueueEmpty){
        sb.append("Empty\n");
    }
    sb.append("PROCESS TABLE\n");
    for(Process pr: processList){
        sb.append("Process " + pr.PID + " started at " + pr.getStartTime() + " ms and is ");
        if(pr.getNextTask() == null){
            sb.append("TERMINATED\n");
        }
        else{
            switch(pr.getLocation()){
                case CORE:
                    sb.append("RUNNING\n");
                    break;
                case DISK:
                    sb.append("BLOCKED - DISK\n");
                    break;
                case DISPLAY:
                    sb.append("BLOCKED - DISPLAY\n");
                    break;
                case DISK_QUEUE:
                case READY_QUEUE:
                case WAITING:
                    sb.append("READY\n");
                break;
            }
       }
    }
    sb.append("\n\n\n\n");
    out.println(sb.toString());
}
