#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef DEBUG
#define BUFF 6
#define OPPS 4
#else
#define BUFF 20
#define OPPS 20
#endif

void
lshift_opps(int opps[OPPS], int index) {
	int i;
	for ( i = index; i<OPPS-1 && opps[i] != 0; i++) {
		opps[i] = opps[i+1];
	}
	opps[i] = -1;
}

void
lshift_bits(int *bits, int len, int index) {
	if (index >= len)
		return;
	int i = index;
	for ( i = index; i<len-1 && bits[i] != 0; i++) {
		bits[i] = bits[i+1];
	}
	bits[i] = -1;
}

unsigned long long
combine(unsigned long long a, unsigned long long b) {

	static int kapow[10] = {
		1, 10, 100, 1000, 10000, 100000,
		100000, 1000000, 10000000, 100000000
	};

	unsigned long long i = b;
	unsigned long long j = 0;

	do {
		i /= 10;
		j++;
	} while (i > 0);

	return (a * kapow[j] + b);
}

int
unify(int opps[OPPS], int len, int *bits) {
	int i;
/*
	printf("unifying:\n");
	for ( i = 0; i<len; i++ ) {
		printf("%d ", opps[i]);
		printf(bits[i] == 0 ? "+ " : (bits[i] == 1 ? "* " : "|| "));
	}
	printf("%d\n", opps[i]);
*/

	for ( i = 0; i<len && opps[i+1] != -1 && bits[i] != -1; i++) {
		if (bits[i] == 2) {
			opps[i] = combine(opps[i], opps[i+1]);
			lshift_opps(opps, i+1);
			lshift_bits(bits, len, i);
			i--;
		}
	}
	printf("to:\n");
	int j;
	for ( j = 0; j < i; j++ ) {
		printf("%d ", opps[j]);
		printf(bits[j] == 0 ? "+ " : bits[j] == 1 ? "* " : "|| ");
	}
	printf("%d\n", opps[j]);
	return i;

}

void
solve_p2(unsigned long long res, unsigned long long opps[OPPS], int len, int *bits, int iter, bool *flag) {

	if (iter == len) {

		unsigned long long opps_tmp[OPPS];
		memcpy(opps_tmp, opps, OPPS*sizeof(unsigned long long));

		int i;

		for ( i = 0; i<len; i++ ) {
			switch (bits[i]) {
			case 0:
				opps_tmp[i+1] = opps_tmp[i] + opps_tmp[i+1];
				break;
			case 1:
				opps_tmp[i+1] = opps_tmp[i] * opps_tmp[i+1];
				break;
			case 2:
				opps_tmp[i+1] = combine(opps_tmp[i], opps_tmp[i+1]);
				break;
			}
		}

		if (opps_tmp[i] == res) {
			// printf("%llu\n", res);
			*flag = true;
		}

		return;
	}

	bits[iter] = 0;
	solve_p2(res, opps, len, bits, iter + 1, flag);

	bits[iter] = 1;
	solve_p2(res, opps, len, bits, iter + 1, flag);

	bits[iter] = 2;
	solve_p2(res, opps, len, bits, iter + 1, flag);
}

unsigned long long
parse_p2(char *line, size_t len) {
	char buff[BUFF] = {0};
	int i;
	for (i = 0; line[i] != ':'; i++)
		buff[i] = line[i];
	unsigned long long res = atoll(buff);
	unsigned long long opps[OPPS] = {0};
	i+=2;
	int j;
	char *str = &(line[i]), *token;
	char *saveptr;
	for ( j = 0; ;j++, str = NULL) {
		token = strtok_r(str, " ", &saveptr);
		if (token == NULL)
			break;
		opps[j] = atoll(token);
	}
	
	bool flag = false;
	int *bits = malloc(sizeof(int) * (j-1));
	memset(bits, 0, j-1);

	solve_p2(res, opps, j-1, bits, 0, &flag);

	free(bits);

	if (flag)
		return res;

	return 0;

}

void
solve(unsigned long long res, int opps[OPPS], int len, int *bits, int iter, bool *flag) {

	if (iter == len) {
		unsigned long long calc = opps[0];
		for (int i = 0; i<len; i++) {
			if (bits[i] == 0) {
				calc += opps[i+1];
			}
			else {
				calc *= opps[i+1];
			}
		}
		// printf("%llu : ", calc);
		if (calc == res) {
		// 	printf("CORRECT\n");
			*flag = true;
		}/*
		else {
			printf("INCORRECT\n");
		}*/
		return;
	}

	bits[iter] = 0;
	solve(res, opps, len, bits, iter + 1, flag);

	bits[iter] = 1;
	solve(res, opps, len, bits, iter + 1, flag);
}

unsigned long long
parse(char *line, size_t len) {
	char buff[BUFF] = {0};
	int i;
	for (i = 0; line[i] != ':'; i++)
		buff[i] = line[i];
	unsigned long long res = atoll(buff);
	int opps[OPPS] = {0};
	i+=2;
	int j;
	char *str = &(line[i]), *token;
	char *saveptr;
	for ( j = 0; ;j++, str = NULL) {
		token = strtok_r(str, " ", &saveptr);
		if (token == NULL)
			break;
		opps[j] = atoi(token);
	}
	
	bool flag = false;
	int *bits = malloc(sizeof(int) * (j-1));
	memset(bits, 0, j-1);

	solve(res, opps, j-1, bits, 0, &flag);

	free(bits);

	if (flag)
		return res;

	return 0;

}

int
main(int argc, char *argv[]){

	FILE *fp;

	#ifdef DEBUG
	fp = fopen("test", "r");
	#else
	fp = fopen("i", "r");
	#endif

	char *line = NULL;
	size_t len = 0;
	size_t nread;
	
	unsigned long long ans = 0;
	unsigned long long ans2 = 0;

	while ( (nread=getline(&line, &len, fp)) != -1 ) {
		// ans += parse(line, nread);	
		ans2 += parse_p2(line, nread);
	}

	printf("PART 1 : %llu\n", ans);
	printf("PART 2 : %llu\n", ans2);

	return 0;
}
