#ifndef FILEIO_H
#define FILEIO_H

#include "main.h"

struct TasksResult {

	task *tasks;
	int count;
};

struct TasksResult readTasks();

#endif
