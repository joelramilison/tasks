#ifndef TASKS_H
#define TASKS_H

#include <stdio.h>


typedef struct Task {
	char title[100];
	int open;
} task;

int getOpenTasks(task *open_tasks, int total_count, task *all_tasks);

#endif

