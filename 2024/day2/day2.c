#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifdef DEBUG
#define COLS  5
#define LINES 6
#else
#define COLS  8
#define LINES 1000
#endif

#define SIGN(x) (x <= 0 ? (x == 0 ? 0 : -1) : 1)
#define ABS(x) (x * SIGN(x))

void
lshift (int *a, int i, int len) {
	for (int j = i; j < len - 1; j++) a[j] = a[j+1];
}

void
printit (int a[LINES][COLS]) {
	for ( int i = 0; i<LINES; ++i) {
		for ( int j = 0; j<COLS; ++j) {
			printf("%d ", a[i][j]);
		}
		putchar('\n');
	}
}

void
printarr (int *a, int len) {
	putchar('[');
	for (int i = 0; i<len-1; ++i)
		printf("%d, ", a[i]);
	printf("%d]", a[len-1]);
}

int
safety(int *l, int len) {
	if ( ABS((l[1] - l[0])) > 3 ) return 0;
	int d = SIGN( (l[1] - l[0]) );
	if (d == 0) return 0;
	for ( int i = 1; i < len - 1; ++i ) {
		int diff = l[i+1] - l[i];
		if (ABS(diff) > 3 || SIGN(diff) != d)
			return 0;
	} 
	return 1;
}

int
safety_dampened(int *l, int len) {
	if ( ABS((l[1] - l[0])) > 3 ) return 0;
	int d = SIGN( (l[1] - l[0]) );
	if (d == 0) return 0;
	for ( int i = 0; i < len - 1; ++i ) {
		int diff = l[i+1] - l[i];
		if (ABS(diff) > 3 || SIGN(diff) != d)
			return i;
	}
	return -1;
} 

int
main(int argc, char *argv[]){

	FILE *fp;

	#ifdef DEBUG
	fp = fopen("test", "r");
	#else 
	fp = fopen("i", "r");
	#endif

	if (fp == NULL)
		return 1;

	char *line = NULL;
	int a[LINES][COLS] = {0};
	size_t len = 0;
	ssize_t nread;

	int i = 0, j;
	char *str, *token, *saveptr;

	int sum = 0;
	int idx;
/* PART 1
	while ((nread = getline(&line, &len, fp)) != -1) {
		for ( j = 0, str = line; ; j++, str = NULL ) {
			token = strtok_r(str, " ", &saveptr);
			if (token == NULL)
				break;
			a[i][j] = atoi(token);
		}
		int s = safety(a[i], j);
		sum += s;
		++i;
	}

	printf("PART 1 : %d\n", sum);
*/

// PART 2
	while ((nread = getline(&line, &len, fp)) != -1) {
		for ( j = 0, str = line; ; j++, str = NULL ) {
			token = strtok_r(str, " ", &saveptr);
			if (token == NULL)
				break;
			a[i][j] = atoi(token);
		}
		int s = safety(a[i], j);
		if (s == 0) {

			int k;
			int tmp[j];

			memcpy(tmp, a[i], sizeof(int) * j);

			for (k = 0; k<j; ++k) {
				lshift(a[i], k, j);
				s = safety(a[i], j-1);
				if (s == 1) {
					break;
				};
				memcpy(a[i], tmp, sizeof(int) * j);
			}

		}
		sum += s;
		++i;
	}

	printf("PART 2 : %d\n", sum);

	free(line);
	fclose(fp);

	return 0;
}
