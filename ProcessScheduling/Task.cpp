#include "Task.h"
#include <iostream>
using namespace std;


Task::Task(TaskType tt, int initialTime){
	type = tt;
	timeLeft = initialTime;
}

TaskType Task::getType(){
	return type;
}

int Task::getTimeLeft(){
	return timeLeft;
}

bool Task::update(){
	timeLeft--;
	return (timeLeft <= 0);
}

bool Task::equals(Task &t){
	return (type == t.getType() && timeLeft == t.getTimeLeft());
}

bool Task::equals(Task *t){
    return (type == t->getType() && timeLeft == t->getTimeLeft());
}