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
	char list[150];
	int list_len;
} CharList; 

typedef
struct {
	int list[50];
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

void
print_solution ( CharList arrangement, IntList group ) {
	for ( int j = 0; j<arrangement.list_len; putc(arrangement.list[j++], stdout));
	putc(' ', stdout);
	for ( int j = 0; j<group.list_len; printf("%d", group.list[j++]));
	putc('\n', stdout);
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
		if (charlist_index == arrangement.list_len) return 0;
		group_size = 0;
		for (group_size = 0; arrangement.list[charlist_index] == '#'
							&& charlist_index < arrangement.list_len; group_size++, charlist_index++ );
		if ( group_size != groups.list[i] ) return 0;
	}
	return 1;
}

void
charlist_cpy(const CharList src, CharList *dst) {
	dst->list_len = src.list_len;
	for ( int i = 0; i<src.list_len; ++i ) {
		(dst->list)[i] = src.list[i];
	}
}

void
intlist_cpy(const IntList src, IntList *dst) {
	dst->list_len = src.list_len;
	for ( int i = 0; i<src.list_len; ++i ) {
		(dst->list)[i] = src.list[i];
	}
}

void
charlist_five_times (CharList *charlist) {
	int prev_len = charlist->list_len;
	CharList tmp;
	charlist_cpy(*charlist, &tmp);
	charlist->list_len = 0;
	for ( int i = 0; i<5; ++i) {
		for ( int j = 0; j<prev_len; ++j) {
			charlist->list[(charlist->list_len)++] = tmp.list[j];
		}
		charlist->list[(charlist->list_len)++] = '?';
	}
}

void
intlist_five_times (IntList *intlist) {
	int prev_len = intlist->list_len;
	IntList tmp;
	intlist_cpy(*intlist, &tmp);
	intlist->list_len = 0;
	for ( int i = 0; i<5; ++i) {
		for ( int j = 0; j<prev_len; ++j) {
			intlist->list[(intlist->list_len)++] = tmp.list[j];
		}
	}
}

void
part_1_solution( CharList arrangements[ROWS], IntList groups[ROWS] ) {

	int N, M, x, y, z, i, *p, *b; // for twiddling
	unsigned long solutions = 0;
	CharList tmp;

	for ( int r = 0; r<ROWS; ++r ) {


		// choosing M elements from N elements
		M = intlist_sum(groups[r]) - char_sum(arrangements[r], '#');
		N = char_sum(arrangements[r], '?');
	
		p = malloc( sizeof(int) * (N+2) );
		b = malloc( sizeof(int) * N );

		inittwiddle(M, N, p);

		for(i = 0; i != N-M; i++) {
			b[i] = 0;
		}
		while(i != N) {
			b[i++] = 1;
		}
		
		charlist_cpy(arrangements[r], &tmp);

		for (int index = 0, i = 0; index < tmp.list_len; ++index) {
			if (tmp.list[index] == '?') {
				tmp.list[index] = (b[i++] ? '#' : '.');
			}
		}
		if (valid_solution(tmp, groups[r])) solutions++;

		while(!twiddle(&x, &y, &z, p))
		{
			charlist_cpy(arrangements[r], &tmp);

			b[x] = 1;
			b[y] = 0;
			for (int index = 0, i = 0; index < tmp.list_len; ++index) {
				if (tmp.list[index] == '?') {
					tmp.list[index] = (b[i++] ? '#' : '.');
				}
			}
			if (valid_solution(tmp, groups[r])) solutions++;
		}

		free(p);
		free(b);
	}
	printf("Part 1 solution: %lu\n", solutions);
}

void
part_2_solution( CharList arrangements[ROWS], IntList groups[ROWS] ) {

	int N, M, x, y, z, i, *p, *b; // for twiddling
	unsigned long solutions = 0;
	CharList tmp;

	for ( int r = 0; r<ROWS; ++r) {
		charlist_five_times(&(arrangements[r]));
		intlist_five_times(&(groups[r]));

		print_solution(arrangements[r], groups[r]);

		// choosing M elements from N elements
		M = intlist_sum(groups[r]) - char_sum(arrangements[r], '#');
		N = char_sum(arrangements[r], '?');
	
		p = malloc( sizeof(int) * (N+2) );
		if ( p == NULL ) {
			printf("Could not allocate %d bytes\n", (N+2) * sizeof(int));
		}
		else {
			printf("Succesfully allocated %d long array of integers for p\n", N+2);
		}
		b = malloc( sizeof(int) * N );
		if ( b == NULL ) {
			printf("Could not allocate %d bytes for b\n", N * sizeof(int) );
		}
		else {
			printf("Succesfully allocated %d long arraoy of integers for b\n", N);
		}

		inittwiddle(M, N, p);

		for(i = 0; i != N-M; i++) {
			b[i] = 0;
		}
		while(i != N) {
			b[i++] = 1;
		}
		
		charlist_cpy(arrangements[r], &tmp);

		for (int index = 0, i = 0; index < tmp.list_len; ++index) {
			if (tmp.list[index] == '?') {
				tmp.list[index] = (b[i++] ? '#' : '.');
			}
		}
		if (valid_solution(tmp, groups[r])) solutions++;

		while(!twiddle(&x, &y, &z, p))
		{
			charlist_cpy(arrangements[r], &tmp);

			b[x] = 1;
			b[y] = 0;
			for (int index = 0, i = 0; index < tmp.list_len; ++index) {
				if (tmp.list[index] == '?') {
					tmp.list[index] = (b[i++] ? '#' : '.');
				}
			}
			if (valid_solution(tmp, groups[r])) solutions++;
		}

		free(p);
		free(b);
	}
	printf("Part 2 solution: %lu\n", solutions);
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

	part_1_solution(arrangements, groups);
	part_2_solution(arrangements, groups);

	free(line);
	fclose(fp);

	return 0;
}
