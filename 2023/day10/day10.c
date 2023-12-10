#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../aoc.h" // for MAX and MIN

#ifdef DEBUG
	#define FILENAME "test"
	#define SIZE 5
	#define START_X 0
	#define START_Y 2
#else
	#define FILENAME "i"
	#define SIZE 140
	#define START_X 17
	#define START_Y 35
#endif

#define putbold(X) printf( "\033[7m%c\033[0m", X )
#define putfaint(X) printf( "\033[1m%c\033[0m", X )

typedef
enum {
	N,
	S,
	W,
	E,
} DIRECTION;

/*
    | -> north and south.
    - -> east and west.
    L -> north and east.
    J -> north and west.
    7 -> south and west.
    F -> south and east.
    . is ground; there is no pipe in this tile.
*/

void
print_maze(char maze[SIZE][SIZE]) {
	for ( int i = 0; i<SIZE; ++i) {
		for (int j = 0; j<SIZE; putc(maze[i][j++], stdout));
		putc('\n', stdout);
	}
}

void
print_loop( char maze[SIZE][SIZE], char loop[SIZE][SIZE] ) {
	for ( int i = 0; i<SIZE; ++i) {
		for ( int j = 0; j<SIZE; ++j) {
			if (loop[i][j])
				putbold(maze[i][j]);
			else
				putc(maze[i][j], stdout);
		}
		putc('\n', stdout);
	}
}

void
part_1_solution( char maze[SIZE][SIZE] ) {

	char loop[SIZE][SIZE];

	int x, y, loop_length;
	DIRECTION d;

	for ( d = N; d < E; ++d) {

		for ( int i = 0; i<SIZE; ++i) {
			for ( int j = 0; j<SIZE; loop[i][j++] = 0);
		}

		x = START_X; y = START_Y;
		loop_length = 0;

		switch (d) {
		case N:
			y = MAX(y-1, 0);
			break;
		case S:
			y = MIN(y+1, SIZE-1);
			break;
		case W:
			x = MAX(x-1, 0);
			break;
		case E:
			x = MIN(x+1, SIZE-1);
			break;
		default:
			break;
		}

		loop[y][x] = 1;
		loop_length++;

		int wall = 0, start = 0;
		while ( !wall && !start ) {
			switch(maze[y][x]) {
			case '|':
				if (d == N) {
					y = y-1;
				} 
				else if (d == S) {
					y = y+1;
				}
				else wall = 1;
				break;
			case '-':
				if (d == E) {
					x = x+1;
				} 
				else if (d == W) {
					x = x-1;
				}
				else wall = 1;
				break;
			case 'L':
				if (d == S) {
					x = x+1;
					d = E;
				}
				else if (d == W) {
					y = y-1;
					d = N;
				}
				else wall = 1;
				break;
			case 'J':
				if (d == E) {
					y = y-1;
					d = N;
				}
				else if (d == S) {
					x = x-1;
					d = W;
				}
				else wall = 1;
				break;
			case '7':
				if (d == E) {
					y = y+1;
					d = S;
				}
				else if (d == N) {
					x = x-1;
					d = W;
				}
				else wall = 1;
				break;
			case 'F':
				if (d == W) {
					y = y+1;
					d = S;
				}
				else if ( d == N ) {
					x = x+1;
					d = E;
				}	
				else wall = 1;
				break;
			case '.':
				wall = 1;
				break;
			default:
				break;
			}

			if (x == SIZE || x == -1 || y == SIZE || y == -1){
				wall = 1;
				break;
			}
			else if ( y == START_Y && x == START_X ) {
				start = 1;
				break;
			}

			loop[y][x] = 1;
			loop_length++;
			/*system("clear");
			print_loop(maze, loop);
			sleep(1);*/
		}

		if (start) {
			printf("%d\n", loop_length);
			print_loop(maze, loop);
			printf("Part 1 solution: %d\n", (loop_length+1)/2);
			return;
		}

	}
}

int
main(int argc, char *argv[]){

	FILE *fp;

	char *line = NULL;
	size_t line_len;
	ssize_t read;

	char maze[SIZE][SIZE];

	fp = fopen(FILENAME, "r");

	int i = 0;

	while ( (read=getline(&line, &line_len, fp)) != -1 ) {
		strncpy(maze[i++], line, SIZE);
	}

	// print_maze(maze);
	part_1_solution(maze);

	free(line);
	fclose(fp);

	return 0;
}
