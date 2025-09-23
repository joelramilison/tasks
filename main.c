#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "fileio.h"


int main() {

	
	struct TasksState state = readTasks();
	showOpenTasksView(&state);

	return 0;
}

void showOpenTasksView(struct TasksState *state) {
	
	task open_tasks[state->count];
	int open_tasks_count = getOpenTasks(open_tasks, state->count, state->tasks);

	if (open_tasks_count == 0) {
		puts("No open tasks.");
		return;
	}

	puts("These are your open tasks:\n");
	for (int i = 0; i < open_tasks_count; i++) {
      		printf("%d. %s\n", i + 1, open_tasks[i]);
	}
	/* TODO:
	printf("\nEnter a comma-separated list of tasks you wish to mark as completed:\n");
	char input[50];
	fgets(input, sizeof(input), stdin);
	puts(input);
	*/

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
