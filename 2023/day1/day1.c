#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#define ISDIGIT(x) ('0' <= x && x <= '9')
#define RESET_BUFFER for(int i = 0; i<6; ++i) buffer[i] = 0
const char NUMBERS[9][6] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
uint8_t
parse_number(char **chr){
	char *chckpnt = *chr;
	for (uint8_t i = 0; i<9; ++i) {
		bool found = true;
		int j = 0;
		do {
			if (**chr != NUMBERS[i][j]){ found = false; break; }
			(*chr)++;
			j++;
		} while (NUMBERS[i][j] != '\0' && **chr != '\n');
		if (!found || (found && (**chr == '\n' && NUMBERS[i][j] != '\0' ) ) ) {
			*chr = chckpnt;
			continue;
		}
		*chr = chckpnt+1;
		return i+1;
	}
	return 255;
}
int
main(int argc, char *argv[]){
	FILE *fp;
	char *line = NULL;
	size_t line_length = 0;
	ssize_t read;
	char buffer[6] = {0};
	int sum = 0;
	fp = fopen("i", "r");
	while ( (read = getline(&line, &line_length, fp)) != -1 ) {
		char *iter;
		uint8_t first = 255;
		uint8_t last  = 255;
		uint8_t value;
		for ( iter = line; *iter != '\n'; iter++ ) {
			if ( ISDIGIT(*iter) ) {
				if (first == 255)
					first = (*iter) - '0';
				last = (*iter) - '0';
			} else {
				uint8_t parsed_number = parse_number(&iter);
				if (parsed_number != 255){
					if (first == 255)
						first = parsed_number;
					last = parsed_number;
				}
			}
		}
		value = first * 10 + last;
		printf("%d\n", value);
		sum += value;
	}
	printf("%d\n", sum);
	return 0;
}
