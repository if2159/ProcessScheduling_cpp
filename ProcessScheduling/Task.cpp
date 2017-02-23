#include <iostream>
#include "Task.h"
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