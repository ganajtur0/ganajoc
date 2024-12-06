#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef DEBUG
#define SIZE 10
#else
#define SIZE 130
#endif

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT
} e_dir;

void
print_map(char map[SIZE][SIZE]) {
	for ( int i = 0; i<SIZE; i++ ) {
		for ( int j = 0; j<SIZE; j++ ) {
			putchar(map[i][j]);
		}
		putchar('\n');
	}
}

void
walk_around(char map[SIZE][SIZE], int init_y, int init_x) {
	e_dir dir = UP;
	int y = init_y;
	int x = init_x;
	while (y >= 0 && y < SIZE && x >= 0 && x < SIZE) {
		map[y][x] = 'X';
		switch (dir) {
			case UP:
				if (map[y-1][x] == '#') dir = RIGHT;
				else y--;
				break;
			case DOWN:
				if (map[y+1][x] == '#') dir = LEFT;
				else y++;
				break;
			case LEFT:
				if (map[y][x-1] == '#') dir = UP;
				else x--;
				break;
			case RIGHT:
				if (map[y][x+1] == '#') dir = DOWN;
				else x++;
				break;
		}
	}
}

bool
is_loop(int intmap[SIZE][SIZE]) {
	int been_there_thrice = 0;
	for ( int i = 0; i<SIZE; i++) {
		for ( int j = 0; j<SIZE; j++) {
			if (intmap[i][j] == 3)
				been_there_thrice++;
		}
	}
	if (been_there_thrice == 4) return true;
	return false;
}

bool
got_stuck(char map[SIZE][SIZE], int init_y, int init_x) {

	int intmap[SIZE][SIZE] = {0};

	e_dir dir = UP;
	int y = init_y;
	int x = init_x;
	while (y > 0 && y < SIZE-1 && x > 0 && x < SIZE-1) {

		intmap[y][x]++;
		if (intmap[y][x] == 3) return true;

		switch (dir) {
			case UP:
				while(map[y-1][x] != '#' && y > 0) y--;
				dir = RIGHT;
				break;
			case DOWN:
				while(map[y+1][x] != '#' && y < SIZE-1) y++;
				dir = LEFT;
				break;
			case LEFT:
				while(map[y][x-1] != '#' && x > 0) x--;
				dir = UP;
				break;
			case RIGHT:
				while(map[y][x+1] != '#' && x < SIZE-1) x++;
				dir = DOWN;
				break;
		}
	}
	return false;
}

int
valid_obstacles(char map[SIZE][SIZE], int init_y, int init_x) {
	int count = 0;
	for ( int i = 0; i<SIZE; i++) {
		for ( int j = 0; j<SIZE; j++) {
			if (map[i][j] == '.') {
				map[i][j] = '#';
				if (got_stuck(map, init_y, init_x)) {
					count++;
				}
				map[i][j] = '.';
			}
		}
	}
	return count;
}

int
count_positions(char map[SIZE][SIZE]) {
	int count = 0;
	for ( int i = 0; i<SIZE; i++) {
		for ( int j = 0; j<SIZE; j++ ) {
			if (map[i][j] == 'X') count++;
		}
	}
	return count;
}

int
main(int argc, char *argv[]){

	char map[SIZE][SIZE] = {0};

	FILE *fp;

	#ifdef DEBUG
	fp = fopen("test", "r");
	#else
	fp = fopen("i", "r");
	#endif

	char c;
	int i = 0, j = 0;
	int init_y, init_x;

	while ((c=fgetc(fp)) != EOF) {
		if (c == '\n') { i++; j=0; }
		else if (c == '^'){
			init_y = i;
			init_x = j;
			map[i][j++] = c;
		}
		else map[i][j++] = c;
	}

	// print_map(map);
	// putchar('\n');
	printf("PART 2: %d\n", valid_obstacles(map, init_y, init_x));

	walk_around(map, init_y, init_x);
	// print_map(map);
	printf("PART 1: %d\n", count_positions(map));

	return 0;
}