#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "fileio.h"


int main() {

	
//	struct TasksState state = readTasks();
//	showOpenTasksView(&state);

/*
	task task1 = {.open = 0, .title = "Hello World!"};
	task task2 = {.open = 0, .title = "Jello World!"};
	task task3 = {.open = 0, .title = "Kello World!"};
	task tasks[3] = {task1, task2, task3};
	struct TasksState state = {.tasks = tasks, .count = 3};
	puts("Before:");
	for (int i = 0; i < state.count; i++) {
		printf("Task %d: %s\n", i + 1, tasks[i].title);
	}


	char *to_delete_titles[2] = {"Hello World!", "Jello World!"};
	int to_delete_count = 2;
	deleteTasks(to_delete_titles, to_delete_count, &state);
	puts("After:");
	for (int i = 0; i < state.count; i++) {
		printf("Task %d: %s\n", i + 1, tasks[i].title);
	}
*/
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
      		printf("%d. %s\n", i + 1, open_tasks[i].title);
	}
	printf("\nEnter a comma-separated list of tasks you wish to mark as completed:\n");

	char input[50];
	long int parsed_choices[50 / 2 + 1];
	int parsed_choices_count;
	int too_high_found;

	// TODO: Add 1 character to the buffer and print an error if it's filled (input too long)
	do {
		too_high_found = 0;
		fgets(input, sizeof(input), stdin);
		parsed_choices_count = parseCommaSepList(input, parsed_choices);

		for (int i = 0; i < parsed_choices_count; i++) {
			if (parsed_choices[i] > open_tasks_count) {
      				fprintf(stderr, "\nOne of the choices exceeds the highest possible task number!\n");
				puts("Please try again:");
				too_high_found = 1;
				break;
			}

		}
		
	} while (too_high_found);

	char *to_delete_titles[parsed_choices_count];
	for (int i = 0; i < parsed_choices_count; i++) {
		to_delete_titles[i] = open_tasks[parsed_choices[i] - 1].title;
	}

	puts("\nDelete the following tasks?\n");
	for (int i = 0; i < parsed_choices_count; i++) {
		printf("- %s\n", to_delete_titles[i]);
	}
	printf("\n[y/n]\n");

	while (1) {

		fgets(input,sizeof(input), stdin);
		if (strcmp(input, "y\n") == 0) {
			deleteTasks(to_delete_titles, parsed_choices_count, state);
			writeTasks(state);	
			puts("\nSuccessfully deleted the tasks.");
			return;
		} else if (strcmp(input, "n\n") == 0) {
			puts("\nAborted the action.");
			return;
		} 
		puts("Wrong input. Try again! [y/n]");
	}
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

	int parsed_numbers_count = 0;
	char *strtol_end;
	char *current = input;

	while (*current != '\0') {
		if (*current == ' ' || *current == ',' || *current == '\n') {
			current++;
			continue;
		}

		parsed_numbers[parsed_numbers_count] = strtol(current, &strtol_end, 0);
		// returns 0 if couldn't parse
		if (parsed_numbers[parsed_numbers_count] == 0) {
			fprintf(stderr, "Error parsing the comma-separated list!\n");
			exit(EXIT_FAILURE);
		}
		parsed_numbers_count++;	
		current = strtol_end; 
	}
	return parsed_numbers_count;
}


// Deletes every task that matches to one of the given titles
void deleteTasks(char **to_delete_titles, int to_delete_count, struct TasksState *state) {

	int found;
	int start_count = state->count;

	for (int i = 0, j = 0; i < start_count; i++) {
      		
      		found = 0;

		if (i != j) {
      			state->tasks[j] = state->tasks[i];
      		}

      		for (int k = 0; k < to_delete_count; k++) {
      			if (strcmp(state->tasks[i].title, to_delete_titles[k]) == 0) {
      				found = 1;
      				state->count--;
      			}
      		}
		
		if (!found) {
      			j++;
		}
	}
}


      				
	

