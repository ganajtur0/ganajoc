#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
// No more reinventing the wheel
#include <ctype.h>

#ifdef DEBUG
	#define WINNING 5
	#define YOURS 8
	#define FILENAME "test"
	#define CARDS 6
#else
	#define WINNING 10
	#define YOURS 25
	#define FILENAME "i"
	#define CARDS 201
#endif

typedef
struct {
	int index;
	int amount;
	int winning[WINNING];
	int w_index;
	int yours[YOURS];
	int y_index;
} Card;

void
print_card( Card *c ) {
	int i;
	printf("Card %3d: ", c->index);
	for ( i = 0; i<WINNING; ++i)
		printf("%2d ", (c->winning)[i]);
	printf("| ");
	for ( i = 0; i<YOURS; ++i)
		printf("%2d ", (c->yours)[i]);
	putc('\n', stdout);
}

void
print_amounts( Card c[CARDS] ) {
	for (int i = 0; i<CARDS; ++i) {
		printf("%d ", c[i].amount);
	}
	putc('\n', stdout);
} 

Card
parse_card(int index, char *line) {

	Card card;

	card.index = index;
	card.amount = 1;
	card.w_index = 0;
	card.y_index = 0;

	char *iter = line;
	while (*iter != ':') { iter++; }; // skip boilerplate
	iter+=2;
	while (*iter != '|'){
		card.winning[card.w_index] = atoi(iter);
		(card.w_index)++;
		iter+=3;
	}
	iter+=2;
	while (*iter != '\0'){
		card.yours[card.y_index] = atoi(iter);	
		(card.y_index)++;
		iter+=3;
	}

	return card;
}

int
get_card_points(Card *c) {
	int score = 0;

	for (int y = 0; y<YOURS; ++y) {
		for (int w = 0; w<WINNING; ++w){
			if ((c->yours)[y] == (c->winning)[w]){
				if (score == 0) score++;
				else score*=2;
				break;
			}
		}
	}

	return score;
}

int
matching_numbers(Card *c) {

	int matches = 0;

	for (int y = 0; y<YOURS; ++y) {
		for (int w = 0; w<WINNING; ++w){
			if ((c->yours)[y] == (c->winning)[w]) {
				matches++;
				break;
			}
		}
	}

	return matches;
}

void
part_1_solution( Card cards[CARDS] ) {
	int sum = 0;
	for ( int c = 0; c < CARDS; ++c) 
		sum += get_card_points(&(cards[c]));
	printf("Part 1 solution: %d\n", sum);
}

void
part_2_solution( Card cards[CARDS] ) {

	int i, j;

	for (i = 0; i<CARDS; ++i) {
		// print_amounts(cards);
		int n = matching_numbers(&(cards[i]));
		for (j = i+1; j <= (i+n); ++j){
			cards[j].amount += cards[i].amount;
		}
	}
	
	int sum = 0;
	for (i = 0; i<CARDS; ++i)
		sum += cards[i].amount;

	printf("Part 2 solution: %d\n", sum);
}

int
main(){

	FILE *fp;

	fp = fopen(FILENAME, "r");

	char *line = NULL;
	size_t line_len;
	ssize_t read;

	int index = 0;
	int sum = 0;

	Card cards[CARDS];
	int cards_index = 0;

	while ( (read = getline(&line, &line_len, fp)) != -1 ) {
		Card card = parse_card(index, line);
		cards[cards_index++] = card;
		index++;
	}

	free(line);
	fclose(fp);

	part_1_solution(cards);
	part_2_solution(cards);

	return 0;
}
