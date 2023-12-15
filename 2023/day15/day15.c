#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef DEBUG
	#define FILENAME "test"
	#define BOX_SIZE 10
#else
	#define FILENAME "i"
	#define BOX_SIZE 100
#endif

typedef
struct {
	char label[10];
	int focal_length;
} Lens;

typedef
struct {
	Lens lenses[BOX_SIZE];
	int lens_count;
} Box;

static inline void
print_lens ( Lens l ) {
	printf("[ %s %d ]", l.label, l.focal_length);
}

static inline int
label_match (char l1[10], char l2[10]) {
	int l1_len; int l2_len;
	for ( l1_len = 0; l1[l1_len] != 0; ++l1_len);
	for ( l2_len = 0; l2[l2_len] != 0; ++l2_len);
	if ( l1_len != l2_len ) return 0;
	for ( int i = 0; i<l1_len; ++i) {
		if (l1[i] != l2[i]) return 0;
	}
	return 1;
}

static inline int
box_contains ( Box box, Lens lens ) {
	for ( int i = 0; i<box.lens_count; ++i ) {
		if (label_match(box.lenses[i].label, lens.label)) return i;
	}
	return -1;
}

void
box_remove( Box *box, int li ) {
	for ( int i = li; i<(box->lens_count-1); ++i) 
		(box->lenses)[i] = (box->lenses)[i+1];
	(box->lens_count)--;
}

void
print_boxes( Box boxes[256] ) {
	for ( int i = 0; i<256; ++i ) {
		if (boxes[i].lens_count > 0) {
			putchar('\n');
			printf( "Box %d: ", i );
			for ( int l = 0; l<boxes[i].lens_count; ++l ) {
				print_lens(boxes[i].lenses[l]);
				putchar(' ');
			}
		}
	}
	putchar('\n');
	putchar('\n');
}

void
operate(char buf[10], Box boxes[256]) {

	int box_index = 0;
	int lens_index;
	Lens l;
	char op;

	for ( int i = 0; i<10 && buf[i] != 0; ++i) {
		if (isalpha(buf[i])) {
			l.label[i] = buf[i];
			box_index = (box_index + buf[i]) * 17 % 256;
		}
		else if (isdigit(buf[i])) {
			l.focal_length = buf[i] - '0';
		}
		else {
			l.label[i] = 0;
			op = buf[i];
		}
	}

	lens_index = box_contains(boxes[box_index], l);
	switch (op) {
		case '=':
			if (lens_index != -1) {
				boxes[box_index].lenses[lens_index].focal_length = l.focal_length;
				break;
			}
			else {
				boxes[box_index].lenses[boxes[box_index].lens_count++] = l;
				break;
			}
		case '-':
			if ( lens_index != -1 ) {
				box_remove(&(boxes[box_index]), lens_index);
			}
			break;
		default:
			break;
	}
}

int
part_2_solution(Box boxes[256]) {
	unsigned long solution = 0;
	for ( int i = 0; i<256; ++i ) {
		if (boxes[i].lens_count > 0) {
			for ( int l = 0; l<boxes[i].lens_count; ++l ) {
				solution += ((i+1) * (l+1) * boxes[i].lenses[l].focal_length);
			}
		}
	}
	return solution;
}

int
main(int argc, char *argv[]){

	FILE *fp = fopen(FILENAME, "r");
	char c;
	int current_value = 0;
	unsigned long result = 0;

	Box boxes[256];
	for (int i = 0; i<256; boxes[i++].lens_count = 0);

	char buf[10];
	int buf_i = 0;

	while ( ( c = fgetc(fp) ) != EOF ) {
		if (c == ',') {
			buf[buf_i] = 0;
			operate(buf, boxes);
			buf_i = 0;
			result += current_value;
			current_value = 0;
			continue;
		}
		buf[buf_i++] = c;
 		current_value += c;
		current_value *= 17;
		current_value = current_value%256;
	}

	buf[buf_i] = 0;
	operate(buf, boxes);
	result += current_value;

	printf("Part 1 solution: %lu\n", result);
	printf("Part 2 solution: %lu\n", part_2_solution(boxes));

	fclose(fp);

	return 0;
}