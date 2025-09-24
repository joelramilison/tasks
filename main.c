#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "fileio.h"


int main() {

	
	struct TasksState state = readTasks();

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
	printf("\nEnter a comma-separated list of tasks you wish to mark as completed:\n");
	char input[50];
	// TODO: Add 1 character to the buffer and print an error if it's filled (input too long)
	fgets(input, sizeof(input), stdin);
	
	

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

int parseCommaSepList(char *input, long int *parsed_numbers) {

	size_t input_length = strlen(input);
	int parsed_numbers_count = 0;
	char *strtol_end;
	char *current = input;

	while (*current != '\0') {
		if (*current == ' ' || *current == ',') {
			current++;
			continue;
		}

		parsed_numbers[parsed_numbers_count] = strtol(current, &strtol_end, 0);
		if (!parsed_numbers[parsed_numbers_count]) {
			fprintf(stderr, "Error parsing the comma-separated list!\n");
			exit(EXIT_FAILURE);
		}
		parsed_numbers_count++;	
		current = strtol_end; 
	}
	return parsed_numbers_count;
}
