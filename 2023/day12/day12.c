#include <stdio.h>
#include <stdlib.h>
#include "../aoc.h"
#include "twiddle.h"

#ifdef DEBUG
	#define FILENAME "test"
	#define ROWS 6
#else
	#define FILENAME "i"
	#define ROWS 1000
#endif // DEBUG

typedef
struct {
	char list[20];
	int list_len;
} CharList; 

typedef
struct {
	int list[10];
	int list_len;
} IntList;

void
parse_data(char **line, CharList *arrangement, IntList *group) {
	arrangement->list_len = 0;
	group->list_len = 0;
	while ( **line != ' ' ) {
		arrangement->list[(arrangement->list_len)++] = *( (*line)++ );
	}
	int could_parse, num;
	num = eat_next_int(line, &could_parse);
	while (could_parse) {
		group->list[(group->list_len)++] = num;
		num = eat_next_int(line, &could_parse);
	}
}

void
print_data(CharList arrangements[ROWS], IntList groups[ROWS]) {
	for ( int i = 0; i<ROWS; ++i ) {
		for ( int j = 0; j<arrangements[i].list_len; putc(arrangements[i].list[j++], stdout));
		putc(' ', stdout);
		for ( int j = 0; j<groups[i].list_len; printf("%d", groups[i].list[j++]));
		putc('\n', stdout);
	}
}

int
intlist_sum(IntList intlist) {
	int acc = 0, i = 0;
	for ( ; i < intlist.list_len; acc+=intlist.list[i++]);
	return acc;
}

int
char_sum(CharList charlist, char c) {
	int acc = 0, i = 0;
	for (; i < charlist.list_len; acc+=(charlist.list[i++] == c ? 1 : 0));
	return acc;
}

int
valid_solution( CharList arrangement, IntList groups ) {
	int charlist_index = 0, group_size;
	for ( int i = 0; i<groups.list_len; ++i ) {
		for (; arrangement.list[charlist_index] == '.' && charlist_index < arrangement.list_len; charlist_index++);
		if (charlist_index == arrangement.list_len-1) return 0;
		group_size = 0;
		for (group_size = 0; arrangement.list[charlist_index] == '#'
							&& charlist_index < arrangement.list_len; group_size++, charlist_index++ );
		if ( group_size != groups.list[i] ) return 0;
	}
	return 1;
}

void
part_1_solution( CharList arrangements[ROWS], IntList groups[ROWS] ) {
	for ( int i = 0; i<ROWS; ++i ) {
		int N, M, x, y, z, *p, *b; // for twiddling
		// choosing M elements from N elements
		M = intlist_sum(groups[i]) - char_sum(arrangements[i], '#');
		N = char_sum(arrangements[i], '?');
		p = malloc( sizeof(int) * (N+2) );
		b = malloc( sizeof(int) * N );
	}
}

int
main(int argc, char *argv[]){

	FILE *fp = fopen(FILENAME, "r");

	char *line = NULL;
	char *saveptr;
	size_t line_len;
	ssize_t read;

	CharList arrangements[ROWS];
	IntList groups[ROWS];

	int index = 0;
	while ( (read = getline(&line, &line_len, fp)) != -1 ) {
		saveptr = line;
		parse_data(&line, &(arrangements[index]), &(groups[index]));
		line = saveptr;
		index++;
	}

	int max = 0;
	for ( int i = 0; i<ROWS; ++i ) {
		int sum = intlist_sum(groups[i]);
		max = (max < sum ? sum : max);
	}
	printf("Max number of ? in an arrangement: %d\n");

	free(line);
	fclose(fp);

	return 0;
}