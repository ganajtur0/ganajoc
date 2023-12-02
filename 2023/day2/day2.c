#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ISDIGIT(x) ('0' <= x && x <= '9')

typedef 
struct {
	int red;
	int blue;
	int green;
} Pull;

typedef 
struct {
	int index;
	int current_pull;
	Pull pulls[20];
} Game;

int
power(Game *g){
	int max_red = 0;
	int max_green = 0;
	int max_blue = 0;
	for (int i = 0; i<g->current_pull; ++i){
		Pull p = g->pulls[i];
		if (max_red < p.red)
			max_red = p.red;
		if (max_green < p.green)
			max_green = p.green;
		if (max_blue < p.blue)
			max_blue = p.blue;
	}

	return max_red * max_green * max_blue;
}

void
print_game(Game g) {
	printf("Game %d: ", g.index);
	for (int i = 0; i<g.current_pull; ++i){
		if (g.pulls[i].red > 0)
			printf("%d red ", g.pulls[i].red);
		if (g.pulls[i].green > 0)
			printf("%d green ", g.pulls[i].green);
		if (g.pulls[i].blue > 0)
			printf("%d blue ", g.pulls[i].blue);
		printf(";");
	}
	printf("\n");
}

bool
is_possible(Game g) {
	for ( int i = 0; i < g.current_pull; ++i)
		if (g.pulls[i].red   > 12 ||
		    g.pulls[i].green > 13 ||
		    g.pulls[i].blue  > 14   )
			return false;
	return true;
}

void
extract_number(char *str, Game *g) {
	for (char *iter = str; *iter != '\0'; ++iter) {
		if (ISDIGIT(*iter)) {
			char buff[3] = {0};
			int i = 0;
			buff[i] = *iter;
			iter++;
			while (ISDIGIT(*iter)){
				i++;
				buff[i] = *iter;
				iter++;
			}
			iter++;
			int num = atoi(buff);
			Pull *p = &(g->pulls[g->current_pull]);
			switch (*iter) {
				case 'r':
					p->red = num;
					break;
				case 'g':
					p->green = num;
					break;
				case 'b':
					p->blue = num;
					break;
				default:
					break;
			} 
			return;
		}
	}
}

Game
parse_game(int index, char *line) {

	// skipping the boilerplate
	char *iter = line;
	while (*iter != ':')
		iter++;

	iter+=2;

	char *str1, *str2, *token, *subtoken;
	char *saveptr1, *saveptr2;
	int j;
	Game g;
	g.current_pull = 0;

	// from the manpage strtok(3)
	for ( j = 1, str1 = iter; /*nothing*/ ; j++, str1 = NULL) {

		token = strtok_r(str1, ";", &saveptr1);
		if (token == NULL) break;

		(g.pulls[g.current_pull]).red = 0;
		(g.pulls[g.current_pull]).green = 0;
		(g.pulls[g.current_pull]).blue = 0;
		
		for (str2 = token; /* nothing */; str2 = NULL){
			subtoken = strtok_r(str2, ",", &saveptr2);
			if (subtoken == NULL) break;

			extract_number(subtoken, &g);
		}
		g.current_pull++;
	}

	g.index = index;

	return g;

}

int
main(){

	FILE *fp;

	char *line = NULL;
	size_t line_len = 0;

	ssize_t read;

	fp = fopen("i", "r");
	if (fp == NULL)
		return 1;


	int index = 1;
	Game g;
	int sum = 0;

	while ( (read = getline(&line, &line_len, fp)) != -1 ) {
		g = parse_game(index, line);
		// pt 2
		sum += power((&g));
		/* pt 1
		if (is_possible(g)){
			sum += index;
		}
		*/
		index++;
	}

	free(line);

	printf("%d\n", sum);

	return 0;
}
