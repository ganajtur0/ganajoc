#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../aoc.h"

#ifdef DEBUG
	#define FILENAME "test"
	#define X 10
#else
	#define FILENAME "i"
	#define X 110
#endif

typedef
enum {
	N,
	W,
	S,
	E
} Direction;

static inline void
contraption_print(char m[X][X]) {
	for ( int i = 0; i<X; ++i) {
		for ( int j = 0; j<X; putchar(m[i][j++]));
		putchar('\n');
	}
}	

void
empty_space( int *x, int *y, Direction *d ) {
	switch (*d) {
		case N:
			(*y)--;
			break;
		case W:
			(*x)--;
			break;
		case S:
			(*y)++;
			break;
		case E:
			(*x)++;
			break;
		default:
			break;
	}
}

// [ / ]
void
mirror_fw( int *x, int *y, Direction *d ) {
	switch (*d) {
		case N:
			*d = E;
			(*x)++;
			break;
		case W:
			*d = S;
			(*y)++;
			break;
		case S:
			*d = W;
			(*x)--;
			break;
		case E:
			*d = N;
			(*y)--;
			break;
		default:
			break;
	}
}

// [ \ ]
void
mirror_bw( int *x, int *y, Direction *d ) {
	switch (*d) {
		case N:
			*d = W;
			(*x)--;
			break;
		case W:
			*d = N;
			(*y)--;
			break;
		case S:
			*d = E;
			(*x)++;
			break;
		case E:
			*d = S;
			(*y)++;
			break;
		default:
			break;
			
	}
}
void
energize ( char contraption[X][X], char energized[X][X], int x, int y, Direction d ) {


	if ( x == X || y == X || x == -1 || y == -1 ) return;
	if (energized[y][x] == '#' && (contraption[y][x] == '|' || contraption[y][x] == '-') ) return;

	int nx = x, ny = y;
	Direction nd = d;

	energized[y][x] = '#';

	switch (contraption[y][x]) {
		case '.':
			empty_space(&nx, &ny, &nd);
			energize(contraption, energized, nx, ny, nd);
			return;
		case '/':
			mirror_fw(&nx, &ny, &nd);
			energize(contraption, energized, nx, ny, nd);
			return;
		case '\\':
			mirror_bw(&nx, &ny, &nd);
			energize(contraption, energized, nx, ny, nd);
			return;
		case '|':
			if ( d == W || d == E ) {
				mirror_fw(&nx, &ny, &nd);
				energize(contraption, energized, nx, ny, nd);
				nx = x; ny = y; nd = d;
				mirror_bw(&nx, &ny, &nd);
				energize(contraption, energized, nx, ny, nd);
				return;
			} else {
				empty_space(&nx, &ny, &nd);
				energize(contraption, energized, nx, ny, nd);
				return;
			}
		case '-':
			if ( d == S || d == N ) {
				mirror_fw(&nx, &ny, &nd);
				energize(contraption, energized, nx, ny, nd);
				nx = x; ny = y; nd = d;
				mirror_bw(&nx, &ny, &nd);
				energize(contraption, energized, nx, ny, nd);
				return;
			} else {
				empty_space(&nx, &ny, &nd);
				energize(contraption, energized, nx, ny, nd);
				return;
			}
		default:
			return;
	}
	return;
}

int
energy_count( char energized[X][X] ) {
	int count = 0;
	for ( int i = 0; i<X; ++i ) {
		for ( int j = 0; j<X; count+=(energized[i][j++] == '#'));
	}
	return count;
}

void
part_1_solution(char contraption[X][X], char energized[X][X]) {
	energize(contraption, energized, 0, 0, E);
	printf("Part 1 solution: %d\n", energy_count(energized));
}

void
energized_reset( char energized[X][X] ) {
	for ( int i = 0; i<X; ++i) {
		for ( int j = 0; j<X; energized[i][j++] = '.' );
	}
}

void
part_2_solution( char contraption[X][X], char energized[X][X] ) {

	energized_reset(energized);

	int max = 0, energy;

	// check top row
	for ( int i = 0; i<X; ++i ) {
		energize(contraption, energized, i, 0, S);
		energy = energy_count(energized);
		if ( energy > max ) max = energy;
		energized_reset(energized);
	}
	// check bottom row
	for ( int i = 0; i<X; ++i ) {
		energize(contraption, energized, i, X-1, N);
		energy = energy_count(energized);
		if ( energy > max ) max = energy;
		energized_reset(energized);
	}
	// check left column
	for ( int i = 0; i<X; ++i ) {
		energize(contraption, energized, 0, i, E);
		energy = energy_count(energized);
		if ( energy > max ) max = energy;
		energized_reset(energized);
	}
	// check right column
	for ( int i = 0; i<X; ++i ) {
		energize(contraption, energized, X-1, i, W);
		energy = energy_count(energized);
		if ( energy > max ) max = energy;
		energized_reset(energized);
	}

	printf("Part 2 solution: %d\n", max);
}

int
main(int argc, char *argv[]){

	FILE *fp = fopen(FILENAME, "r");
	char c;
	char contraption[X][X];
	char energized[X][X];
	for ( int i = 0; i<X; ++i) {
		for ( int j = 0; j<X; energized[i][j++] = '.' );
	}
	int i = 0, j = 0;

	while ( ( c = fgetc(fp) ) != EOF ) {
		if ( c == '\n') {
			i++;
			j = 0;
			continue;
		}
		contraption[i][j++] = c;
	}

	part_1_solution(contraption, energized);
	part_2_solution(contraption, energized);

	fclose(fp);

	return 0;
}
