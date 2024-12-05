#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef DEBUG
#define RULES 6
#define PAGES 5
#else
#define RULES 24
#define PAGES 24
#endif

bool
rule_contains(int rule[RULES], int a) {
	for ( int i = 0; i<RULES; i++) {
		if (a == rule[i])
			return true;
	}
	return false;
}

void
swap(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int
check_pages(int pages[PAGES], int len, int ruleset[100][RULES]) {
	for ( int i = 0; i<len-1; i++ ) {
		if (!rule_contains(ruleset[pages[i]], pages[i+1])) {
			return 0;
		}
	}
	return pages[(len/2)];
}

int
correct_order(int pages[PAGES], int len, int ruleset[100][RULES]) {
	bool toggle = false;
	for ( int i = 0; i<len-1; i++ ) {
		if (!rule_contains(ruleset[pages[i]], pages[i+1])) {
			toggle = true;
			swap(&(pages[i]), &(pages[i+1]));
		}
	}
	if (toggle) {
		for ( int j = 0; j<len; j++ ) {
			printf("%d ", pages[j]);
		}
	}
	putchar('\n');
	return toggle ? pages[(len/2)] : 0;
}

void
print_ruleset(int ruleset[100][RULES]) {
	for ( int i = 0; i < 100; i++) {
		if (ruleset[i][0] == 0) continue;
		printf("%d : ", i);
		for ( int j = 0; j < RULES && ruleset[i][j] != 0; j++ )
			printf("%d ", ruleset[i][j]);
		putchar('\n');
	}
}

int
parse_line(char *line, size_t len, int ruleset[100][RULES], int buffer[PAGES]) {

	if (len == 6) {
		
		char buff[3] = {0};
		int rule_index;
		int rule;

		buff[0] = line[0];
		buff[1] = line[1];
		rule_index = atoi(buff);
		buff[0] = line[3];
		buff[1] = line[4];
		rule = atoi(buff);

		int i = 0;
		while (ruleset[rule_index][i++] != 0);
		i--;

		ruleset[rule_index][i] = rule;

		return 0;
	}
	else if (len == 1) {
		return 0;
	}
	else {

		char *str, *token;
		char *saveptr;
		int i;

		for (i = 0, str = line; ; i++, str=NULL) {

			token = strtok_r(str, ",", &saveptr);
			if (token == NULL) {
				return i;
			}
			buffer[i] = atoi(token);

		}
	}

}

int
main(int argc, char *argv[]){
	
	FILE *fp;

	#ifdef DEBUG
	fp = fopen("test", "r");
	#else
	fp = fopen("i", "r");
	#endif

	int ruleset[100][RULES] = {0};
	int buffer[PAGES];

	int buff_len;

	char *line = NULL;
	size_t len;
	size_t nread;

	bool process_start = false;

	int sum = 0;
	int ordered_sum = 0;

	while ((nread = getline(&line, &len, fp)) != -1) {
		buff_len = parse_line(line, nread, ruleset, buffer);
		if (process_start) {
			sum += check_pages(buffer, buff_len, ruleset);
			ordered_sum += correct_order(buffer, buff_len, ruleset);
		}
		else if (line[0] == '\n') {
			process_start = true;
		}
	}
	
	printf("PART 1: %d\n", sum);
	printf("PART 2: %d\n", ordered_sum);

	return 0;
}
