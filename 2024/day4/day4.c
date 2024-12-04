#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#define SIZE 10
#else
#define SIZE 140
#endif

void
print_array(char **a, size_t size) {
	for (int i = 0; i<size; i++) {
		for (int j = 0; j<size-1; j++) {
			printf("%c ", a[i][j]);
		}
		printf("%c\n", a[i][size-1]);
	}
}

int
horizontal(char a[SIZE][SIZE]) {
	int count = 0;
	char slice[4];
	for ( int i = 0; i<SIZE; i++ ) {
		for ( int j = 0; j<SIZE-3; j++ ) {
			for ( int k = 0; k<4; k++) {
				slice[k] = a[i][j+k];
			}
			if (strncmp(slice, "XMAS", 4) == 0 ||
			    strncmp(slice, "SAMX", 4) == 0) {
				count++;
				// printf("found one at row %d and col %d\n", i, j);
			}
		}
	}
	return count;
}

int
vertical(char a[SIZE][SIZE]) {
	int count = 0;
	char slice[4];
	for ( int i = 0; i<SIZE-3; i++ ) {
		for ( int j = 0; j<SIZE; j++ ) {
			for ( int k = 0; k<4; k++) {
				slice[k] = a[i+k][j];
			}
			if (strncmp(slice, "XMAS", 4) == 0 ||
			    strncmp(slice, "SAMX", 4) == 0) {
				count++;
				// printf("found one at row %d and col %d\n", i, j);
			}
		}
	}
	return count;
}

int
diagonal(char a[SIZE][SIZE]) {
	int count = 0;
	char slice[4];
	for ( int i = 0; i<SIZE-3; i++) {
		for ( int j = 0; j<SIZE-3; j++) {
			for ( int k = 0; k<4; k++) {
				slice[k] = a[i+k][j+k];
			}
			if (strncmp(slice, "XMAS", 4) == 0 ||
			    strncmp(slice, "SAMX", 4) == 0) {
				count++;
			}
		}
	}
	for ( int i = 0; i<SIZE; i++) {
		for ( int j = SIZE-1; j>=3; j--) {
			for ( int k = 0; k < 4; k++) {
				slice[k] = a[i+k][j-k];
			}
			if (strncmp(slice, "XMAS", 4) == 0 ||
			    strncmp(slice, "SAMX", 4) == 0) {
				count++;
			}
		}
	}
	return count;
}

int
xmas(char a[SIZE][SIZE]) {
	int count = 0;
	char frame[3][3];
	char slice1[3];
	char slice2[3];

	for ( int i = 0; i<SIZE-2; i++) {
		for ( int j = 0; j<SIZE-2; j++) {
			for ( int k = 0; k<3; k++) {
				for ( int l = 0; l<3; l++){
					frame[k][l] = a[i+k][j+l];
				}
			}
			for ( int k = 0; k<3; k++) {
				slice1[k] = frame[k][k];
				slice2[k] = frame[2-k][k];
			}
			if ( (strncmp(slice1, "MAS", 3) == 0 || strncmp(slice1, "SAM", 3) == 0) &&
			     (strncmp(slice2, "MAS", 3) == 0 || strncmp(slice2, "SAM", 3) == 0) ) {
				count++;
				// printf("found one at row %d and col %d\n", i, j);
			}
		}
	}
	return count;
}

int
main(int argc, char *argv[]){

	char ws[SIZE][SIZE] = {0};

	FILE *fp;

	#ifdef DEBUG
	fp = fopen("test", "r");
	#else
	fp = fopen("i", "r");
	#endif

	char *line = NULL;
	size_t line_len;

	for (int i = 0; getline(&line, &line_len, fp) != -1; i++)
		memcpy(ws[i], line, SIZE);

	#ifdef PART1
	int count = horizontal(ws);
	count += diagonal(ws);
	count += vertical(ws);
	printf("PART 1: %d\n", count);
	#else
	int count = xmas(ws);
	printf("PART 2: %d\n", count);
	#endif


	return 0;
}
