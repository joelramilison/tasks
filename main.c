#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "fileio.h"


int main() {

	

	return 0;
}

int getOpenTasks(task *open_tasks, int total_count, task *all_tasks) {

	int open_counter = 0;
	int i;
	for (i = 0; i < total_count; i++) {
		if (all_tasks[i].open) {
			open_tasks[open_counter] = all_tasks[i];
			open_counter++;
		}
	}
	return open_counter;
}
