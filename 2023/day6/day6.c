#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../aoc.h"

#ifdef DEBUG
	#define FILENAME "test"
	#define RACES 3
#else
	#define FILENAME "i"
	#define RACES 4
#endif 

typedef
struct {
	int time;
	int distance;
} Race;

void
parse_races( char **line1, char **line2, Race races[RACES], size_t race_index ) {
	int num, i = 0;
	while ( (num = eat_next_int(line1)) != -1 )
		races[i++].time = num;
	i = 0;
	while ( (num = eat_next_int(line2)) != -1 )
		races[i++].distance = num;
}

void
print_races( Race races[RACES] ) {
	int i;
	printf( "Time:\t" );
	for ( i = 0; i<RACES; ++i)
		printf( "%3d ", races[i].time );
	printf( "\nDistance:\t" );
	for ( i = 0; i<RACES; ++i)
		printf( "%3d ", races[i].distance );
	putc('\n', stdout);
}

void
part_1_solution( Race races[RACES] ) {
	int distance, record_beating, result = 1;
	for ( int i = 0; i<RACES; ++i) {
		record_beating = 0;
		for ( int j = 0; j<races[i].time; ++j ) {
			distance = ((races[i].time - j) * j);
			if (distance > races[i].distance)
				record_beating++;
		}
		result *= record_beating;
	}
	printf("Part 1 solution: %d\n", result);
}

void
part_2_solution( char **line1, char **line2) {

	unsigned long long time = eat_the_rich(line1);
	unsigned long long distance = eat_the_rich(line2);

	unsigned long long record_beating = 0;

	for ( unsigned long long j = 0; j<time; ++j ) {
		if (distance < (( time - j ) * j) )
			record_beating++;
	}
	printf("Part 2 solution: %llu\n", record_beating);
}

int
main(int argc, char *argv[]){

	FILE *fp = fopen(FILENAME, "r");

	char *line1 = NULL;
	size_t line1_len;

	char *line2 = NULL;
	size_t line2_len;
	
	Race races[RACES];
	size_t race_index = 0;

	getline(&line1, &line1_len, fp);
	getline(&line2, &line2_len, fp);

	char *saveptr1 = line1;
	char *saveptr2 = line2;

	parse_races(&line1, &line2, races, race_index);

	line1 = saveptr1;
	line2 = saveptr2;

	part_2_solution(&line1, &line2);

	free(saveptr1);
	free(saveptr2);

	part_1_solution(races);

	return 0;
}
