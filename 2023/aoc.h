#ifndef AOC_H
#define AOC_H

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static inline int
line_is_empty( char *line ) {
	while ( *line != '\0' ){
		line++;
		if (!isspace(*line))
			return 0;
	}
	return 1;
}

unsigned long long
eat_the_rich(char **line) {
	char buffer[100] = {0};
	while (!isdigit(**line)) {
		(*line)++;
		if (**line == '\n' || **line == '\0')
			return -1;
	}
	for (int i = 0; **line != '\0'; (*line)++) {
		if (isdigit(**line))
			buffer[i++] = **line;
	}
	return strtoull(buffer, NULL, 10);
}

unsigned long
eat_next_unsigned_long(char **line) {

	char buffer[20] = {0};
	while (!isdigit(**line)) {
		(*line)++;
		if (**line == '\n' || **line == '\0')
			return -1;
	}
	for (int i = 0; !(isspace(**line)); buffer[i++] = **line, (*line)++);
	return strtoul(buffer, NULL, 10);
}

int
eat_next_int(char **line) {

	char buffer[20] = {0};
	while (!isdigit(**line)) {
		(*line)++;
		if (**line == '\n' || **line == '\0')
			return -1;
	}
	for (int i = 0; !(isspace(**line)); buffer[i++] = **line, (*line)++);
	return atoi(buffer);
}

#endif //AOC_H
