#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
	#define FILENAME "test"
	#define SIZE 10
	#define EXPANDED_ROWS 12
	#define EXPANDED_COLS 13
#else
	#define FILENAME "i"
	#define SIZE 140
	#define EXPANDED_ROWS 146
	#define EXPANDED_COLS 147 
#endif

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
				putc(image_expanded[exp_row][k], stdout);
			}
			putc('\n', stdout);
			exp_row++;
		}
		for (int j = 0; j<SIZE && j<EXPANDED_COLS; ++j, ++exp_col) {
			if (col_empty(image, j)) {
				image_expanded[exp_row][exp_col] = '.';
				putc(image_expanded[exp_row][exp_col], stdout);
				exp_col++;
			}
			image_expanded[exp_row][exp_col] = image[i][j];
			putc(image_expanded[exp_row][exp_col], stdout);
		}
		putc('\n', stdout);
	}
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

	int i = 0;

	while ( (read = getline(&line, &line_len, fp)) != -1 ) {
		strncpy(image[i++], line, SIZE);
	}

	print_image(image);
	printf("Empty rows: %d\n", empty_rows(image));
	printf("Empty columns: %d\n", empty_cols(image));
	cosmic_expansion(image, image_expanded);
	print_expanded_image(image_expanded);
	free(line);
	fclose(fp);

	return 0;
}
