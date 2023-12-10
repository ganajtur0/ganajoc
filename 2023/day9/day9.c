#include <stdio.h>
#include <stdlib.h>
#include "../aoc.h"

#ifdef DEBUG
	#define FILENAME "test"
	#define LENGTH 8
	#define LINES 3
#else
	#define FILENAME "i"
	#define LENGTH 30
	#define LINES 200
#endif //DEBUG

typedef
struct {
	int array[LENGTH];
	int length;
} IntStack;

void
parse_row( char **line, IntStack *stack ) {
	int num, could_parse;
	stack->length = 0;
	do {
		num = eat_next_int(line, &could_parse);

		if (!could_parse) return;

		stack->array[stack->length++] = num;
	} while ( 1 );
}

static inline void
intstack_print( IntStack is ) {
	for (int i = 0; i<is.length; printf( "%d ", is.array[i++] ));
	putc('\n', stdout);
}

int
next_value( IntStack history ) {
	IntStack diff;
	diff.length = history.length - 1;
	int allzeros = 1;
	for (int i = 1; i<history.length; ++i) {
		int d = history.array[i]-history.array[i-1];
		if (d != 0) allzeros = 0;
		diff.array[i-1] = d;
	}
	if (allzeros) return history.array[history.length-1];
	return (history.array[history.length-1] + next_value(diff));
}

int
next_value_pt2( IntStack history ) {
	IntStack diff;
	diff.length = history.length - 1;
	int allzeros = 1;
	for (int i = 1; i<history.length; ++i) {
		int d = history.array[i]-history.array[i-1];
		if (d != 0) allzeros = 0;
		diff.array[i-1] = d;
	}
	if (allzeros) return history.array[0];
	// should name these functions better
	return (history.array[0] - next_value_pt2(diff));
}

void
part_1_solution ( IntStack values[LINES] ) {
	int acc = 0;
	for (int i = 0; i<LINES; acc+=next_value(values[i++]) );
	printf("Part 1 solution: %d\n", acc);
}

void
part_2_solution ( IntStack values[LINES] ) {
	int acc = 0;
	for (int i = 0; i<LINES; acc+=next_value_pt2(values[i++]) );
	printf("Part 2 solution: %d\n", acc);
}

int
main(int argc, char *argv[]){

	FILE *fp;
	char *line;
	char *svptr;
	size_t len;
	ssize_t read;

	fp = fopen(FILENAME, "r");

	IntStack values[LINES];
	int value_index = 0;

	while ( (read = getline(&line, &len, fp)) != -1 ) {
		svptr = line;
		parse_row(&line, &(values[value_index++]));
		line = svptr;
	}

	// this print statement is crucial for the testcase to not get segfaulted. cool
	// for ( int i = 0; i<LINES; intstack_print(values[i++]) );

	// part_1_solution(values);
	part_2_solution(values);

	free(line);
	fclose(fp);

	return 0;
}
