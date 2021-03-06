#include <string.h>
#include "string_util.h"

void sanitize_string(char *input) {
	static char ok_chars[] = "abcdefghijklmnopqrstuvwxyz"
							 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
							 "1234567890_-.@";
	char *cp = input;
	const char *end = input + strlen(input);
	for (cp += strspn(cp, ok_chars); cp != end; cp += strspn(cp, ok_chars)) {
		*cp = '_';
	}
}

int sanitize_number(char *input) {
	static char ok_chars[] = "1234567890.-";
	char *cp = input;
	const char *end = input + strlen(input);
	for (cp += strspn(cp, ok_chars); cp != end; cp += strspn(cp, ok_chars)) {
		return(-1);
	}

	return(0);
}

