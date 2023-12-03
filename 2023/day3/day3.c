#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef 
struct {
	uint8_t x;
	uint8_t y;
} XY;

typedef 
struct {
	uint8_t x1;
	uint8_t x2;
	uint8_t y1;
	uint8_t y2;
} Range;

typedef 
struct {
	XY xy;
	uint8_t adj_to;
	int ratio;
} Gear;

#define ISDIGIT(x) ('0' <= x && '9' >= x)
#define MAX(x,y)   ( x>y ? x : y )
#define MIN(x,y)   ( x<y ? x : y )

#ifdef DEBUG
	#define X 10
	#define FILENAME "test"
	#define GEARS 3
#else
	#define X 140
	#define FILENAME "i"
	#define GEARS 391
#endif //DEBUG

void
print_schematic(char s[X][X]) {
	for (uint8_t i = 0; i<X; ++i) {
		for (uint8_t j = 0; j<X; ++j) {
			putc(s[i][j], stdout);
		}
		putc('\n', stdout);
	}
}

bool
gear_in_gears( uint8_t x, uint8_t y, Gear gears[GEARS], int *index ) {
	for ( int i = 0; i < GEARS; ++i) {
		if (x == gears[i].xy.x && y == gears[i].xy.y){
			*index = i;
			return true;
		}
	}
	return false;
}

int
find_next_number(char s[X][X], XY *current_pos, Range *r) {

	uint8_t x;
	uint8_t y;
	uint8_t x2;
	
	int number;

	char buffer[4] = {0};

	x = current_pos->x; y = current_pos->y;

	bool found = false;

	for (/*nothing*/; y < X; ++y){
		while (x < X) {
			if (ISDIGIT(s[y][x])) {
				found = true;
				x2 = x;
				do {
					buffer[x2-x] = s[y][x2];
					x2++;
				} while (ISDIGIT(s[y][x2]));
				x2--;
				number = atoi(buffer);
				break;
			}
			x++;
		}
		if (found) break;
		x = 0;
		
	}

	if (!found) return -1;

	r->x1 = x;
	r->y1 = y;
	r->x2 = x2;
	r->y2 = y;

	return number;
}

bool
symbol_adjacent(char s[X][X], Range *r) {
	for ( uint8_t y = MAX( (r->y1)-1, 0); y < MIN( (r->y2)+2, X); ++y){
		for ( uint8_t x = MAX( (r->x1)-1, 0); x < MIN( (r->x2)+2, X); ++x){
			if ( !ISDIGIT(s[y][x]) && s[y][x] != '.' ){
				return true;
			}
		}
	}
	return false;
}

void
update_gears(char s[X][X], Range *r, int  num, Gear gears[GEARS], int *gear_len) {
	int index;
	for ( uint8_t y = MAX( (r->y1)-1, 0); y < MIN( (r->y2)+2, X); ++y){
		for ( uint8_t x = MAX( (r->x1)-1, 0); x < MIN( (r->x2)+2, X); ++x){
			if ( !ISDIGIT(s[y][x]) && s[y][x] == '*' ){
				if (gear_in_gears(x, y, gears, &index)) {
					(gears[index].adj_to)++;
					(gears[index].ratio) *= num;
				}
				else {

					XY xy;
					xy.x = x;
					xy.y = y;

					Gear g;
					g.ratio = num;
					g.xy = xy;
					g.adj_to = 1;

					gears[(*gear_len)++] = g;
				}
			}
		}
	}
}

int
part1_solution(char s[X][X]) {
	XY xy;
	xy.x = 0;
	xy.y = 0;

	Range r;

	int num;
	int sum = 0;

	while ( (num = find_next_number(s, &xy, &r)) != -1 ){
		if (symbol_adjacent(s, &r)) {
			sum += num;
		}
		xy.x = (r.x2 == (X-1) ? 0 : r.x2+1);
		xy.y = (r.x2 == (X-1) ? r.y2+1 : r.y2);
	}

	return sum;

}

int
part2_solution(char s[X][X], Gear gears[GEARS], int *gear_len) {
	
	XY xy;
	xy.x = 0;
	xy.y = 0;

	Range r;

	int num;

	while ( (num = find_next_number(s, &xy, &r)) != -1 ){
		update_gears(s, &r, num, gears, gear_len);
		xy.x = (r.x2 == (X-1) ? 0 : r.x2+1);
		xy.y = (r.x2 == (X-1) ? r.y2+1 : r.y2);
	}

	int sum = 0;

	for ( int i = 0; i<GEARS; ++i )
		if (gears[i].adj_to == 2) sum += gears[i].ratio;

	return sum;

}

int
main(){

	FILE *f;

	f = fopen(FILENAME, "r");

	char *row = NULL;
	
	size_t row_len;
	ssize_t read;

	char schematic[X][X];

	uint8_t i = 0;

	while ( (read = getline(&row, &row_len, f)) != -1 ){
		for (uint8_t j = 0; j<X; ++j)
			schematic[i][j] = row[j];
		i++;
	}

	free(row);

	printf("%d\n", part1_solution(schematic));

	// counted the stars
	Gear gears[GEARS];
	int gear_len = 0;

	printf( "%d\n", part2_solution(schematic, gears, &gear_len) );

	return 0;
}
