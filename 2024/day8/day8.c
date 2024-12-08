#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define SIZE 12
#else
#define SIZE 50
#endif

#define MAX(X, Y) (X > Y ? X : Y)
#define MIN(X, Y) (X < Y ? X : y)
#define ABS(X) ((X) < 0 ? (X) * -1 : (X))

void
printit(char map[SIZE][SIZE]) {
	for ( int i = 0; i<SIZE; i++) {
		for ( int j = 0; j<SIZE; j++) {
			putchar(map[i][j]);
		}
		putchar('\n');
	}
}

int
part_one(char map[SIZE][SIZE], char antinodes[SIZE][SIZE]) {
	int count = 0;
	for ( int i = 0; i<SIZE; i++ ) {
		for ( int j = 0; j<SIZE; j++) {
			if (map[i][j] != '.') {
				for ( int k = i; k<SIZE; k++ ) {
					for ( int l = (k == i ? j+1 : 0); l<SIZE; l++) {
						if (map[k][l] == map[i][j]) {
							int r_diff = k - i;
							int c_diff = j - l;
							int r1 = i - r_diff;
							int r2 = k + r_diff;
							int c1 = j + c_diff;
							int c2 = l - c_diff;
							if ( r1 >= 0 && r1 < SIZE && c1 >= 0 && c1 < SIZE ) {
								// count+=(antinodes[r1][c1] == '.' ? 1 : 0);
								antinodes[r1][c1] = '#';
								// count++;
							}
							if ( r2 >= 0 && r2 < SIZE && c2 >= 0 && c2 < SIZE ) {
								// count+=(antinodes[r1][c1] == '.' ? 1 : 0);
								antinodes[r2][c2] = '#';
								// count++;
							}
						}
					}
				}
			}
		}
	}
	for ( int i = 0; i<SIZE; i++) {
		for ( int j = 0; j<SIZE; j++) {
			if (antinodes[i][j] == '#') {
				count++;
				if (map[i][j] == '.') map[i][j] = '#';
			}
		}
	}
	return count;
}

int
part_two(char map[SIZE][SIZE], char antinodes[SIZE][SIZE]) {
	int count = 0;
	for ( int i = 0; i<SIZE; i++ ) {
		for ( int j = 0; j<SIZE; j++) {
			if (map[i][j] != '.') {
				for ( int k = i; k<SIZE; k++ ) {
					for ( int l = (k == i ? j+1 : 0); l<SIZE; l++) {
						if (map[k][l] == map[i][j]) {

							antinodes[k][l] = '#';
							antinodes[i][j] = '#';

							int r_diff = k - i;
							int c_diff = j - l;
							int r1 = i - r_diff;
							int r2 = k + r_diff;
							int c1 = j + c_diff;
							int c2 = l - c_diff;
							while ( r1 >= 0 && r1 < SIZE && c1 >= 0 && c1 < SIZE ) {
								// count+=(antinodes[r1][c1] == '.' ? 1 : 0);
								antinodes[r1][c1] = '#';
								r1 -= r_diff;
								c1 += c_diff;
								// count++;
							}
							while ( r2 >= 0 && r2 < SIZE && c2 >= 0 && c2 < SIZE ) {
								// count+=(antinodes[r1][c1] == '.' ? 1 : 0);
								antinodes[r2][c2] = '#';
								r2 += r_diff;
								c2 -= c_diff;
								// count++;
							}
						}
					}
				}
			}
		}
	}
	for ( int i = 0; i<SIZE; i++) {
		for ( int j = 0; j<SIZE; j++) {
			if (antinodes[i][j] == '#') {
				count++;
				if (map[i][j] == '.') map[i][j] = '#';
			}
		}
	}
	return count;
}

int
main(int argc, char *argv[]){

	FILE *fp;

	#ifdef DEBUG
	fp = fopen("test", "r");
	#else
	fp = fopen("i", "r");
	#endif

	if (fp == NULL) {
		return 1;
	}

	char map[SIZE][SIZE] = {0};
	char antinodes[SIZE][SIZE];

	for ( int i = 0; i<SIZE; i++) {
		for ( int j = 0; j<SIZE; j++) {
			antinodes[i][j] = '.';
		}
	}

	char x;
	int r = 0, c = 0;
	while ( (x = getc(fp)) != EOF  ) {
		if (x == '\n') {
			r++; c = 0;
		}
		else {
			map[r][c++] = x;
		}
	}

	// printf("PARt 1 : %d\n", part_one(map, antinodes));
	printf("PARt 2 : %d\n", part_two(map, antinodes));

	printit(map);

	fclose(fp);

	return 0;
}
