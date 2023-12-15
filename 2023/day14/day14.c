#include <stdio.h>
#include <stdlib.h>
#include "../aoc.h"

#ifdef DEBUG
	#define FILENAME "test"
	#define X 10
#else
	#define FILENAME "i"
	#define X 100
#endif

int
total_load (char** dish) {
	int total = 0;
	for ( int i = 0; i<X; ++i ) {
		for ( int j = 0; j<X; ++j ) {
			if (dish[i][j] == 'O') total+=(X-i);
		}
	}
	return total;
}

void
part_1_solution(char dish[X][X]) {
	int acc = 0;
	for ( int i = 0; i<X; ++i ) {
		for ( int j = 0; j<X; ++j ) {
			if ( dish[j][i] == 'O' ) {
				int y = 0;
				for ( y = j-1; y>=0; --y ) {
					if ( dish[y][i] == '#' || dish[y][i] =='O' ) {
						y+=1; break;
					}
				}
				if (y<0) y = 0;
				acc+=(X-y);
				dish[j][i] = '.';
				dish[y][i] = 'O';
			}
		}
	}
	printf("Part 1 solution: %d\n", acc);
}

int
dish_compare(char** d1, char d2[X][X]) {
	for ( int i = 0; i<X; ++i ) {
		for ( int j = 0; j<X; ++j ) {
			if (d1[i][j] != d2[i][j]) return 0;
		}
	}
	return 1;
}

void
cycle(char dish[X][X]) {
	// NORTH
	for ( int i = 0; i<X; ++i ) {
		for ( int j = 0; j<X; ++j ) {
			if ( dish[j][i] == 'O' ) {
				int y = 0;
				for ( y = j-1; y>=0; --y ) {
					if ( dish[y][i] == '#' || dish[y][i] =='O' ) {
						y+=1; break;
					}
				}
				if (y<0) y = 0;
				dish[j][i] = '.';
				dish[y][i] = 'O';
			}
		}
	}
	
	// WEST
	for ( int i = 0; i<X; ++i ) {
		for ( int j = 0; j<X; ++j ) {
			if ( dish[i][j] == 'O' ) {
				int x = 0;
				for ( x = j-1; x>=0; --x ) {
					if ( dish[i][x] == '#' || dish[i][x] =='O' ) {
						x+=1; break;
					}
				}
				if (x<0) x = 0;
				dish[i][j] = '.';
				dish[i][x] = 'O';
			}
		}
	}
	//SOUTH
	for ( int i = 0; i<X; ++i ) {
		for ( int j = X-1; j>=0; --j ) {
			if ( dish[j][i] == 'O' ) {
				int y = 0;
				for ( y = j+1; y<X; ++y ) {
					if ( dish[y][i] == '#' || dish[y][i] =='O' ) {
						y-=1; break;
					}
				}
				if (y>=X) y = X-1;
				dish[j][i] = '.';
				dish[y][i] = 'O';
			}
		}
	}
	//EAST
	for ( int i = 0; i<X; ++i ) {
		for ( int j = X-1; j>=0; --j ) {
			if ( dish[i][j] == 'O' ) {
				int x = 0;
				for ( x = j+1; x<X; ++x ) {
					if ( dish[i][x] == '#' || dish[i][x] =='O' ) {
						x-=1; break;
					}
				}
				if (x>=X) x = X-1;
				dish[i][j] = '.';
				dish[i][x] = 'O';
			}
		}
	}
}

void
print_dish(char dish[X][X]) {
	for ( int i = 0; i<X; ++i ) {
		for ( int j = 0; j<X; ++j ) {
			putchar(dish[i][j]);
		}
		putchar('\n');
	}

}

int
main(int argc, char *argv[]){

	char dish[X][X];
	FILE *fp = fopen(FILENAME, "r");
	char *line = NULL;
	size_t line_len;
	ssize_t read;

	int i = 0;
	while ( (read = getline(&line, &line_len, fp)) != -1 ) {
		int j = 0;
		for ( char *iter = line; *iter != '\n' && j < X; dish[i][j++] = *(iter++) );
		++i;
	}

	putchar('\n');

	// the stack cannot handle a statically allocated char [1000][100][100] ( which is reasonable )
	char*** states = (char***)malloc( sizeof(char**) * 1000 );
	for ( int i = 0; i<1000; ++i ) {
		states[i] = (char**)malloc( sizeof(char*) * X );
		for ( int j = 0; j<X; ++j ) {
			states[i][j] = (char*)malloc( X );
		}
	}

	int c, _c, repeat = 0; 
	for ( c = 0; c<1000; ++c ) {
		for ( int i = 0; i<X; ++i ) {
			for ( int j = 0; j<X; ++j ) {
				states[c][i][j] = dish[i][j];
			} 
		}
		cycle(dish);
		for ( _c = 0; _c < c; ++_c ) {
			if ( dish_compare(states[_c], dish) ) {
				repeat = 1;
				printf("dish after cycle %d = state at cycle %d\n", c+1, _c);
				break;
			}
		}
		if (repeat)
			break;
	}

	// again - thanks to hyper-neutrino
	printf("%d\n", total_load(states[ (1000000000 - _c) % (c+1 - _c) + _c ]));

	for ( int i = 0; i<1000; ++i ) {
		for ( int j = 0; j<X; ++j ) {
			free(states[i][j]);
		}
		free(states[i]);
	}
	free(states);

	return 0;
}
