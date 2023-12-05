#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// THEY DON'T FIT INTO AN INT!!!!!
// THEY DON'T FIT INTO A unsigned long!!!!!

#ifdef DEBUG
	#define FILENAME "test"
	#define RANGES 5
	#define SEEDS 4
#else
	#define FILENAME "i"
	#define RANGES 100
	#define SEEDS 20
#endif

typedef
struct {
	unsigned long dst_start;
	unsigned long src_start;
	unsigned long length;
} Range;

typedef 
struct {
	Range ranges[RANGES];
	int r_index;
} Map;

static inline int
line_is_empty( char *line ) {
	while ( *line != '\0' ){
		line++;
		if (!isspace(*line))
			return 0;
	}
	return 1;
}

unsigned long
eat_next_unsigned_long(char **line) {

	char buffer[20] = {0};
	while (!isdigit(**line)) {
		(*line)++;
		if (**line == '\n' || **line == '\0')
			return -1;
	}
	for (int i = 0; !(isspace(**line)); buffer[i++] = **line, (*line)++);
	return strtoul(buffer, NULL, 10);
}

void
parse_seeds(char **line, unsigned long seeds[SEEDS]) {
	unsigned long i = 0, num;
	while ( (num = eat_next_unsigned_long(line)) != -1) {
		seeds[i++] = num;
	}
}

Range
parse_range(char **line) {
	Range r;
	unsigned long dst_start = eat_next_unsigned_long(line);
	unsigned long src_start = eat_next_unsigned_long(line);
	unsigned long length = eat_next_unsigned_long(line);
	r.dst_start = dst_start;
	r.src_start = src_start;
	r.length = length;
	return r;
}

void
print_unsigned_long_array(unsigned long array[], size_t length) {
	printf("[ ");
	for (int i = 0; i < length; ++i )
		printf("%lu ", array[i]);
	printf("]\n");
}

static inline void
print_range(Range r) {
	printf("%lu ", r.dst_start);
	printf("%lu ", r.src_start);
	printf("%lu\n", r.length);
}

static inline void
print_maps(Map maps[7]) {
	for (int i = 0; i<7; ++i) {

		printf("\n%d =====\n", i);

		for (int j = 0; j<maps[i].r_index; ++j)
			print_range(maps[i].ranges[j]);
	}
}

static inline int
in_range(unsigned long x, Range r) {
	if (r.src_start <= x && x <= ((r.src_start) + r.length) )
		return 1;
	return 0;
}

unsigned long
mapped_value(unsigned long x, Map m) {
	for (int i = 0; i<m.r_index; ++i) {
		if (in_range(x, m.ranges[i]))
			return x + (m.ranges[i].dst_start - m.ranges[i].src_start);
	}
	return x;
}

unsigned long
map_seed(unsigned long seed, Map maps[7]) {
	unsigned long location = seed;
	for (int i = 0; i < 7; ++i) {
		location = mapped_value(location, maps[i]);
	}
	return location;
}

void
part_1_solution(unsigned long seeds[SEEDS], Map maps[7]) {
	unsigned long smallest_value = ULONG_MAX;
	for ( int i = 0; i<SEEDS; ++i) {
		unsigned long value = map_seed(seeds[i], maps);
		if (value < smallest_value)
			smallest_value = value;
	}
	printf("Part 1 solution: %lu\n", smallest_value);
}

void
part_2_solution(unsigned long seeds[SEEDS], Map maps[7]) {
	unsigned long smallest_value = ULONG_MAX;

	for (int i = 0; i < SEEDS; i+=2) {

		for ( unsigned long j = seeds[i]; j< (seeds[i] + seeds[i+1]); ++j) {
			unsigned long value = map_seed(j, maps);
			if (value < smallest_value)
				smallest_value = value;
		}
		
	}
	printf("Part 2 solution: %lu\n", smallest_value);
}

int
main(int argc, char *argv[]){

	FILE *fp = fopen(FILENAME, "r");

	char *line = NULL;
	size_t line_len;

	ssize_t read;

	Map maps[7];
	int maps_index = 0;

	for ( int i = 0; i < 7; ++i ) {
		Map m;
		m.r_index = 0;
		maps[i] = m;
	}

	unsigned long seeds[SEEDS];

	char *svptr;

	read = getline(&line, &line_len, fp);

	svptr = line; // cause we modify line in-place

	parse_seeds(&line, seeds);
	free(svptr);
	
	/* skip two lines */
	line = NULL;
	getline(&line, &line_len, fp);
	line = NULL;
	getline(&line, &line_len, fp);

	line = NULL;

	while ( (read = getline(&line, &line_len, fp)) != -1 ) {
		if (*line == '\n') {
			maps_index++;
			continue;
		}
		else if ( !isdigit(*line) )
			continue;

		svptr = line;
		Range r = parse_range(&line);
		line = svptr;

		maps[maps_index].ranges[(maps[maps_index].r_index)++] = r;
	}

	free(line);
	fclose(fp);

	part_1_solution(seeds, maps);
	part_2_solution(seeds, maps);

	return 0;
}
