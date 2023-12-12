#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../aoc.h"

#ifdef DEBUG
	#define FILENAME "test"
	#define SIZE 10
	#define EXPANDED_ROWS 12
	#define EXPANDED_COLS 13
	#define NUMBER_OF_GALAXIES 9
#else
	#define FILENAME "i"
	#define SIZE 140
	#define EXPANDED_ROWS 146
	#define EXPANDED_COLS 147 
	#define NUMBER_OF_GALAXIES 443
#endif

typedef
struct {
	int x;
	int y;
} Point;

void
print_image(char image[SIZE][SIZE]) {
	for (int i = 0; i<SIZE; ++i) {
		for (int j = 0; j<SIZE; putc(image[i][j++], stdout));
		putc('\n', stdout);
	}
}

void
print_expanded_image( char exp_image[EXPANDED_ROWS][EXPANDED_COLS] ) {
	for (int i = 0; i<EXPANDED_ROWS; ++i) {
		for (int j = 0; j<EXPANDED_COLS; putc(exp_image[i][j++], stdout));
		putc('\n', stdout);
	}
}

int
empty_rows(char image[SIZE][SIZE]) {
	int count = 0;
	int empty;
	for (int i = 0; i<SIZE; ++i) {
		empty = 1;
		for (int j = 0; j<SIZE; ++j)
			if (image[i][j] == '#') empty = 0;
		if (empty)
			count++;
	}
	return count;
}

int
empty_cols(char image[SIZE][SIZE]) {
	int count = 0;
	int empty;
	for ( int i = 0; i<SIZE; ++i) {
		empty = 1;
		for (int j = 0; j<SIZE; ++j)
			if (image[j][i] == '#') empty = 0;
		if (empty)
			count++;
	}
	return count;
}

int
row_empty(char image[SIZE][SIZE], int y) {
	for ( int i = 0; i<SIZE; ++i) {
		if (image[y][i] != '.') return 0;
	}
	return 1;
}


int
col_empty(char image[SIZE][SIZE], int x) {
	for (int i = 0; i<SIZE; ++i) {
		if (image[i][x] != '.') return 0;
	}
	return 1;
}

void
cosmic_expansion(char image[SIZE][SIZE], char image_expanded[EXPANDED_ROWS][EXPANDED_COLS]) {
	int exp_col = 0;
	int exp_row = 0;
	for (int i = 0; i<SIZE && exp_row<EXPANDED_ROWS; ++i, ++exp_row) {
		if (row_empty(image, i)) {
			for ( int k = 0; k<EXPANDED_COLS; ++k){
				image_expanded[exp_row][k] = '.';
			}
			exp_row++;
		}
		for (int j = 0, exp_col = 0; j<SIZE && j<EXPANDED_COLS; ++j, ++exp_col) {
			if (col_empty(image, j)) {
				image_expanded[exp_row][exp_col] = '.';
				exp_col++;
			}
			image_expanded[exp_row][exp_col] = image[i][j];
		}
	}
}

int
number_of_galaxies(char image_expanded[EXPANDED_ROWS][EXPANDED_COLS]) {
	int num = 1;
	for ( int i = 0; i<EXPANDED_ROWS; ++i) {
		for ( int j = 0; j<EXPANDED_COLS; ++j) {
			if (image_expanded[i][j] == '#') {
				// image_expanded[i][j] = (num+'0');
				num++;
			}
		}
	}
	return num;
}

void
collect_galaxies( char image_expanded[EXPANDED_ROWS][EXPANDED_COLS], Point galaxies[NUMBER_OF_GALAXIES] ) {
	int num = 0;
	for ( int i = 0; i<EXPANDED_ROWS; ++i ) {
		for ( int j = 0; j<EXPANDED_COLS; ++j ) {
			if (image_expanded[i][j] == '#') {
				galaxies[num].y = i;
				galaxies[num].x = j;
				num++;
			}
		}
	} 
}

void
collect_galaxies_pt2( char image [SIZE][SIZE], Point galaxies[NUMBER_OF_GALAXIES]) {
	int num = 0;
	for ( int i = 0; i<SIZE; ++i ) {
		for ( int j = 0; j<SIZE; ++j ) {
			if (image[i][j] == '#') {
				galaxies[num].y = i;
				galaxies[num].x = j;
				num++;
			}
		}
	}
}

static inline void
print_point( Point p ) {
	printf("(%d %d)", p.x, p.y);
}

void
part_1_solution( char image_expanded[EXPANDED_ROWS][EXPANDED_COLS], Point galaxies[NUMBER_OF_GALAXIES] ) {
	printf("Number of pairs: %d\n", ( NUMBER_OF_GALAXIES * (NUMBER_OF_GALAXIES-1) ) / 2);
	unsigned long total_distance = 0;
	for ( int i = 0; i<NUMBER_OF_GALAXIES; ++i ) {
		for ( int j = 0; j<i; ++j) {
			for ( int y = MIN(galaxies[i].y, galaxies[j].y); y<MAX(galaxies[i].y, galaxies[j].y); ++y, ++total_distance);
			for ( int x = MIN(galaxies[i].x, galaxies[j].x); x<MAX(galaxies[i].x, galaxies[j].x); ++x, ++total_distance);
		}
	}
	printf("Part 1 solution: %lu\n", total_distance);
}

// hyper-neutrino solved i watched the video
// otherwise i woulda been brute forcing
void
part_2_solution( char image[SIZE][SIZE], Point galaxies[NUMBER_OF_GALAXIES] ) {
	unsigned long total_distance = 0;
	for ( int i = 0; i<NUMBER_OF_GALAXIES; ++i ) {
		for ( int j = 0; j<i; ++j) {
			for ( int y = MIN(galaxies[i].y, galaxies[j].y);
				y<MAX(galaxies[i].y, galaxies[j].y); ++y, total_distance+=(row_empty(image, y) ? 1000000 : 1) );
			for ( int x = MIN(galaxies[i].x, galaxies[j].x);
				x<MAX(galaxies[i].x, galaxies[j].x); ++x, total_distance+=(col_empty(image, x) ? 1000000 : 1) );
		}
	}
	printf("Part 1 solution: %lu\n", total_distance);
}

int
main(int argc, char *argv[]){

	FILE *fp;
	char *line = NULL;
	size_t line_len;
	ssize_t read;

	fp = fopen(FILENAME, "r");

	char image[SIZE][SIZE];
	char image_expanded[EXPANDED_ROWS][EXPANDED_COLS];

	Point galaxies[NUMBER_OF_GALAXIES];

	int i = 0;

	while ( (read = getline(&line, &line_len, fp)) != -1 ) {
		strncpy(image[i++], line, SIZE);
	}

	cosmic_expansion(image, image_expanded);
	collect_galaxies(image_expanded, galaxies);
	part_1_solution(image_expanded, galaxies);
	collect_galaxies_pt2(image, galaxies);
	part_2_solution(image, galaxies);

	free(line);
	fclose(fp);

	return 0;
}
