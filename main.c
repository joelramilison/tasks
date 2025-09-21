#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "fileio.h"


int main() {

	struct TasksResult result = readTasks();
	printf("Found %d tasks:\n\n", result.count);
	for (int i = 0; i < result.count; i++) {
		printf("%s (Open: %d)\n", result.tasks[i].title, result.tasks[i].open);
	}
	free(result.tasks);
	

	return 0;
}
