#ifndef TASKS_H
#define TASKS_H

#include <stdio.h>


typedef struct Task {
	char title[100];
	int open;
} task;

struct TasksState {

	task *tasks;
	int count;
	int tasks_allocated;
};

int getOpenTasks(task *open_tasks, int total_count, task *all_tasks);

void showOpenTasksView();

int parseCommaSepList(char *input, long int *parsed_numbers);

#endif

