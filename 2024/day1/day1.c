#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX(x, y) ( x > y ? x : y )
#define MIN(x, y) ( x < y ? x : y )

#ifdef TEST
#define SIZE 6
#else
#define SIZE 1000
#endif

int
smallest(int *a) {
	int s = INT_MAX;
	int j;
	for ( int i = 0; i<SIZE; ++i) {
		if (a[i] < s) {
			s = a[i];
			j = i;
		}
	}
	a[j] = INT_MAX;
	return s;
}

unsigned long
similarity(int *a, int *b) {
	unsigned long score = 0;
	for (int i = 0; i<SIZE; ++i) {
		for (int j = 0; j<SIZE; ++j) {
			if (b[j] == a[i])
				score += a[i];
		}
	}
	return score;
}

void
printit(int *a) {
	putchar('[');
	for ( int i = 0; i < SIZE-1; ++i) {
		printf("%d, ", a[i]);
	}
	printf("%d", a[SIZE-1]);
	putchar(']');
}

int
main(int argc, char *argv[]){

	FILE *fp;

	#ifdef TEST
	fp = fopen("test", "r");
	#else
	fp = fopen("i", "r");
	#endif

	if (fp == NULL)
		return 0;

	int i = 0;
	int r[SIZE] = {0}, l[SIZE] = {0};
	int r_min, l_min;
	unsigned long sum = 0;

	for ( int i = 0; fscanf(fp, "%d   %d\n", &(l[i]), &(r[i])) != EOF; ++i);

	printf("PART 2 -- score : %lu\n", similarity(l, r));

	#ifdef TEST
	printit(l); putchar('\n'); printit(r); putchar('\n');
	#endif

	while (1) {
		l_min = smallest(l);
		r_min = smallest(r);

		#ifdef TEST
		printf("%d %d\n", l_min, r_min);
		#endif

		if (l_min == INT_MAX || r_min == INT_MAX)
			break;

		sum += MAX(l_min, r_min) - MIN(l_min, r_min);
	}

	printf("PART 1 -- sum : %lu\n", sum);

	return 0;
}
