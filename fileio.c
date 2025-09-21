#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "fileio.h"

const char DB_PATH[] = "data/.db";

struct TasksResult readTasks() {

	FILE *db_file = fopen(DB_PATH, "r");
	if (db_file == NULL) {
		struct TasksResult result;
		result.tasks = NULL;
		result.count = 0;
		return result;
	}

	// Read tasks count from first line
     	char tasks_count_buffer[10];
	if (fgets(tasks_count_buffer, sizeof(tasks_count_buffer), db_file) == NULL) {
		fprintf(stderr, "Couldn't read first line from file.\n");
		exit(EXIT_FAILURE);
	}
	int tasks_count;
	if (sscanf(tasks_count_buffer, "%d", &tasks_count) != 1) {
		fprintf(stderr, "Couldn't parse tasks count. Stopping execution.\n");
		exit(EXIT_FAILURE);
	}

	// Read tasks list
	task *tasks = malloc(tasks_count * sizeof(task));
	// Title: 100, open + comma: 2, newline: 1
      	char buffer[104];
	for (int i = 0; i < tasks_count; i++) {
      		if (fgets(buffer, sizeof(buffer), db_file) == NULL) {
      			fprintf(stderr, "Error reading tasks from database file: Reader returned NULL before the last task\n");
      			exit(EXIT_FAILURE);
      		}
      		tasks[i].open = (buffer[0] == '1');

		// Offset to skip the "open" boolean
      		for (int j = 2; j < 104; j++) {
			// Don't write the \n into the task title
      			if (buffer[j] == '\0' || buffer[j] == '\n') {
		    		tasks[i].title[j - 2] = '\0';
				break;
			}
			// If the max. buffer character still isn't newline
			else if (j == 102 && buffer[j] != '\n') {
				fprintf(stderr, "Error: Task title of task %d too long. (max. 100 characters)\n", i + 1);
				exit(EXIT_FAILURE);
			} else {
      				tasks[i].title[j - 2] = buffer[j];
			}
		}
      	}
	if (fgets(buffer, sizeof(buffer), db_file) != NULL) {
		fprintf(stderr, "Error: More lines in the file than specified.\n");
		exit(EXIT_FAILURE);
	}
	struct TasksResult result = {.tasks = tasks, .count = tasks_count};
	fclose(db_file);
      	return result;
}
