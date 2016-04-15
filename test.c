#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "string_util.h"

int get_number(void) {
	char input[9];
	fgets(input, sizeof(input), stdin);
	input[strlen(input)-1] = '\0';

	if (sanitize_number(input) == -1) {
		printf("FATAL ERROR: Invalid entry\n");
		exit(1);
	}

	return(atoll(input));
}

char *get_string(void) {
	char input[50];
	fgets(input, sizeof(input), stdin);
	input[strlen(input)-1] = '\0';

	sanitize_string(input);

	char *output = strdup(input);

	return(output);
}
