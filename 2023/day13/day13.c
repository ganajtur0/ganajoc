// TODO: do an epic comeback, and solve part 2!

#include <stdio.h>
#include <stdlib.h>
#include "../aoc.h"

#ifdef DEBUG
	#define FILENAME "test"
	#define MAPS 2
#else
	#define FILENAME "i"
	#define MAPS 100
#endif

int
reflection( CharMatrix cm ) {
	
	int acc = 0;
	int found;

	// horizontal
	for ( int i = 0; i<(cm.line_num-1); ++i ) {
		found = 1;
		for ( int j = i, k = i+1; j>=0 && k<cm.line_num; --j, ++k ){
			for ( int l = 0; l<cm.width; ++l ) {
				if ( cm.matrix[j][l] != cm.matrix[k][l] ) {
					found = 0;
					break;
				}	
			}
			if (!found) break;
		}
		if ( found ) {
			printf("%d\n", (i+1)*100);
			acc += (i+1)*100;
		}
	}

	// vertical
	for ( int i = 0; i<(cm.width-1); ++i ) {
		found = 1;
		for ( int j = i, k = i+1; j>=0 && k<cm.width; --j, ++k ){
			for ( int l = 0; l<cm.line_num; ++l ) {
				if ( cm.matrix[l][j] != cm.matrix[l][k] ) {
					found = 0;
					break;
				}	
			}
			if (!found) break;
		}
		if ( found ){
			printf("%d\n", i+1);
			acc += i+1;
		}
	}

	return acc;
}

void
part_1_solution( CharMatrix maps[MAPS] ) {
	unsigned long acc = 0;
	for ( int i = 0; i<MAPS; ++i) {
		//printf("\n%d\n", i);
		//putchar('\n');
		int r = reflection(maps[i]);
		acc+=r;
		//printf("\n%d\n", r);
		charmatrix_print(maps[i]);
	}
	printf("Part 1 solution: %lu\n", acc);
}

int
main(int argc, char *argv[]){

	CharMatrix maps[MAPS];

	FILE *fp = fopen(FILENAME, "r");
	char *line = NULL;
	size_t line_len;
	ssize_t read;

	int map_index = -1;
	int create_new = 1;
	while ( (read = getline(&line, &line_len, fp)) != -1 ) {
		if ( *line == '\n' ) {
			create_new = 1; continue;
		};
		if (create_new) {
			maps[++map_index] = charmatrix_create(read-1, 0);
			create_new = 0;
		}
		charmatrix_add_line(&(maps[map_index]), line, '\n');
	}

	part_1_solution(maps);

	for ( int i = 0; i<MAPS; ++i) {
		charmatrix_free(&(maps[i]));
	}

	free(line);
	fclose(fp);

	return 0;
}