#ifndef FILEIO_H
#define FILEIO_H

#include "main.h"


struct TasksState readTasks();

void writeTasks(struct TasksState *state);

#endif
