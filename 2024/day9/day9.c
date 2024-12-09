#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#ifdef DEBUG
#define LEN 19
#else
#define LEN 19999
#endif

void
print_expanded(int *expanded, int n) {
	for ( int i = 0; i<n; i++ ) {
		if (expanded[i] == -1) putchar('.');
		else printf("%d ", expanded[i]);
	}
	putchar('\n');
}

int *
expand( uint8_t disk_map[LEN], int *length ) {
	int sum = 0;
	int i;
	for ( i = 0; i < LEN; i++) sum+=disk_map[i];
	int *expanded = malloc(sum * sizeof(int));
	*length = sum;
	int id = 0, pos = 0;
	for ( i = 0; i < LEN; i++) {
		if ( !(i % 2) ) {
			for ( int j = 0; j<disk_map[i]; j++ )
				expanded[pos++] = id;
			id++;
		}
		else {
			for ( int j = 0; j<disk_map[i]; j++ )
				expanded[pos++] = -1;
		}
	}	
	return expanded;
}

void
compress( int *expanded, int n ) {
	for ( int i = n-1; i>=0; i-- ) {
		if (expanded[i] == -1 ) continue;
		int j;
		for ( j = 0; expanded[j] != -1 && j < i; j++);
		if ( !(j >= i) ) {
			expanded[j] = expanded[i];
			expanded[i] = -1;
		}
		else break;
	}
}

void
compress_p2( int *expanded, int n ) {
	int j = 0;
	int id = INT_MAX;
	int i = n-1;
	while (id > 0) {

		while ( i >= 0 && expanded[i] == -1 ) i--;

		id = expanded[i];
		int block_size = 0;

		int block_start = i;
		for ( ; block_start >= 0 && expanded[block_start] == id; block_start-- ) block_size++;
		block_start++;

		int j_tmp = j;
		while (1) {
			int free_space_size = 0;
			for ( ; expanded[j] != -1 && j < i; j++);
			if ( j >= i ) break;
			int free_space_start = j;
			for ( ; j < i && expanded[j] == -1; j++) free_space_size++;

			// printf("id : %d\t block start : %d\tblock size : %d\tfree space start : %d\tfree space size %d\n", id, block_start, block_size, free_space_start, free_space_size);

			if (free_space_size >= block_size) {
				for ( int block = 0; block<block_size; block++ ) {
					expanded[free_space_start + block] = expanded[block_start + block];
					expanded[block_start + block] = -1;
				}
				break;
			}
		}
		i = block_start - 1;
		j = j_tmp;
		// print_expanded(expanded, n);
	}
}

unsigned long long
checksum(int *expanded, int n) {
	unsigned long long cs = 0;
	for ( int i = 0; i<n; i++ ) {
		if ( expanded[i] == -1 ) break;
		cs += ( i * expanded[i] );
	}
	return cs;
}

unsigned long long
checksum_p2(int *expanded, int n) {
	unsigned long long cs = 0;
	for ( int i = 0; i<n; i++ ) {
		cs += (expanded[i] == -1 ? 0 : (i * expanded[i]));
	}
	return cs;
}

int
main(int argc, char *argv[]){

	FILE *fp;

	#ifdef DEBUG
	fp = fopen("test", "r");
	#else
	fp = fopen("i", "r");
	#endif

	if (fp == NULL) return 1;

	uint8_t disk_map[LEN];

	char c;
	int i = 0;
	while ( (c = fgetc(fp)) != EOF ) {
		disk_map[i++] = c - '0';
	}
	fclose(fp);

	int n;
	int *expanded = expand(disk_map, &n);


	// compress(expanded, n);
	compress_p2(expanded, n);

	// printf("PART 1 : %llu\n", checksum(expanded, n));
	printf("PART 2 : %llu\n", checksum_p2(expanded, n));

	return 0;
}
