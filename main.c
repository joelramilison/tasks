#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "fileio.h"


int main(int argc, char **argv) {

	struct TasksState state = readTasks();
	if (argc == 1) {
		showOpenTasksView(&state);
	} else if (argc > 3) {
		fprintf(stderr, "Too many arguments given. Try again.\n");
	} else if (argc == 3 && strcmp(argv[1], "-a") == 0) {
		addTask(argv[2], &state);
	} else if (argc == 2 && strcmp(argv[1], "-c") == 0) {
		clearOpenTasks(&state);
	} else if (argc == 2 && strcmp(argv[1], "-p") == 0) {
		startTaskPlanning(&state);
	}

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
	
	task *open_tasks[state->count];
	int open_tasks_count = getOpenTasks(open_tasks, state);

	if (open_tasks_count == 0) {
		puts("No open tasks.");
		return;
	}

	puts("These are your open tasks:\n");
	for (int i = 0; i < open_tasks_count; i++) {
      		printf("%d. %s\n", i + 1, open_tasks[i]->title);
	}
	printf("\nEnter a comma-separated list of tasks you wish to mark as completed.\n");
	printf("To change nothing, enter 'n'.\n");

	char input[50];
	long int parsed_choices[50 / 2 + 1];
	int parsed_choices_count;
	int too_high_found;

	// TODO: Add 1 character to the buffer and print an error if it's filled (input too long)
	do {
		too_high_found = 0;
		fgets(input, sizeof(input), stdin);
		if (strcmp(input, "n\n") == 0) {
			puts("Exiting the program.");
			exit(EXIT_SUCCESS);
		}
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
		to_delete_titles[i] = open_tasks[parsed_choices[i] - 1]->title;
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

void clearOpenTasks(struct TasksState *state) {

	for (int i = 0; i < state->count; i++) {
		state->tasks[i].open = 0;
	}
	writeTasks(state);
	puts("Successfully cleared open tasks.");
}

int getOpenTasks(task **open_tasks, struct TasksState *state) {

	int open_counter = 0;
	int i;
	for (i = 0; i < state->count; i++) {
		if (state->tasks[i].open) {
			open_tasks[open_counter] = &(state->tasks[i]);
			open_counter++;
		}
	}
	return open_counter;
}

void startTaskPlanning(struct TasksState *state) {

	if (state->count == 0) {
		puts("Your task list is empty.");
		return;
	}
	
	task *open_tasks[state->count];
	int open_tasks_count = getOpenTasks(open_tasks, state);

	puts("Your open tasks:\n");
	if (open_tasks_count == 0) {
		puts("No open tasks.");
	}

	task *listed_tasks[state->count];
	int list_counter = 0;

	for (int i = 0; i < open_tasks_count; i++) {
		listed_tasks[list_counter] = open_tasks[i];
		list_counter++;
		printf("%d. %s\n", list_counter, open_tasks[i]->title);
	}
	
	puts("\nOther tasks:\n");

	for (int i = 0; i < state->count; i++) {
		int found = 0;
		for (int j = 0; j < open_tasks_count; j++) {
			if (&(state->tasks[i]) == open_tasks[j]) {
				found = 1;
			}
		}
		if (!found) {
			listed_tasks[list_counter] = &(state->tasks[i]);
			list_counter++;
			printf("%d. %s\n", list_counter, state->tasks[i].title);
		}
	}

	puts("Enter a comma-separated list of tasks to plan or unplan.");
	puts("(Entering 'n' stops the program)\n");

	
	char input[50];
	long int parsed_choices[50 / 2 + 1];
	int parsed_choices_count;
	int too_high_found;

	// TODO: Add 1 character to the buffer and print an error if it's filled (input too long)
	do {
		too_high_found = 0;
		fgets(input, sizeof(input), stdin);
		if (strcmp(input, "n\n") == 0) {
			puts("Exiting the program.");
			exit(EXIT_SUCCESS);
		}
		parsed_choices_count = parseCommaSepList(input, parsed_choices);

		for (int i = 0; i < parsed_choices_count; i++) {
			if (parsed_choices[i] > list_counter) {
      				fprintf(stderr, "\nOne of the choices exceeds the highest possible task number!\n");
				puts("Please try again:");
				too_high_found = 1;
				break;
			}

		}
		
	} while (too_high_found);

	task *to_modify_tasks[parsed_choices_count];
	for (int i = 0; i < parsed_choices_count; i++) {
		to_modify_tasks[i] = listed_tasks[parsed_choices[i] - 1];
	}

	puts("\nPlan/Unplan the following tasks?\n");
	for (int i = 0; i < parsed_choices_count; i++) {
		printf("- %s\n", to_modify_tasks[i]->title);
	}
	printf("\n[y/n]\n");

	while (1) {

		fgets(input,sizeof(input), stdin);
		if (strcmp(input, "y\n") == 0) {
			for (int i = 0; i < parsed_choices_count; i++) {
				to_modify_tasks[i]->open = 1 - to_modify_tasks[i]->open;	
			}
			writeTasks(state);	
			puts("\nSuccessfully planned/unplanned the tasks.");
			return;
		} else if (strcmp(input, "n\n") == 0) {
			puts("\nAborted the action.");
			return;
		} 
		puts("Wrong input. Try again! [y/n]");
	}
	

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


void addTask(char *title, struct TasksState *state) {

	if (strlen(title) > 100) {
		fprintf(stderr, "Error: Title too long.\n");
		exit(EXIT_FAILURE);
	}
	int i = 0;
	char temp = '0';
	while (temp != '\0') {
		if (temp == '\n') {
			fprintf(stderr, "Error: Line breaks in task name.");
			exit(EXIT_FAILURE);
		}
		temp = title[i];
		i++;
	}
	int new_tasks_allocated = (state->count > 20) ? 2 * state->count : 40;

	if (new_tasks_allocated != state->tasks_allocated) {
		state->tasks_allocated = new_tasks_allocated;
		state->tasks = realloc(state->tasks, new_tasks_allocated * sizeof(task));
		if (state->tasks == NULL) {
			fprintf(stderr, "Error reallocating memory after adding a task.\n");
			exit(EXIT_FAILURE);
		}
	}
	
	task *added_task = &(state->tasks[state->count]);

	strncpy(added_task->title, title, 100);
	added_task->open = 0;
	added_task->title[100 - 1] = '\0';
	state->count += 1;
	writeTasks(state);
	printf("Successfully added task:\n%s\n", title);
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


      				
	

