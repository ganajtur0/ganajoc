#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "../aoc.h"

#ifdef DEBUG
	#define FILENAME "test"
	#define HANDS 5
#else
	#define FILENAME "i"
	#define HANDS 1000
#endif

typedef
struct {
	char cards[5];
	int bid;
} Hand;

static inline int
lettervalue(char letter) {
	switch (letter) {
		case 'A':
			return 14;
		case 'K':
			return 13;
		case 'Q':
			return 12;
		case 'J':
			return 11;
		case 'T':
			return 10;
		default:
			return 0;
	}
}


static inline int
lettervalue_pt2(char letter) {
	switch (letter) {
		case 'A':
			return 14;
		case 'K':
			return 13;
		case 'Q':
			return 12;
		case 'J':
			return  1; 
		case 'T':
			return 10;
		default:
			return  0;
	}
}

typedef
enum {
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	FIVE_OF_A_KIND,
} HandValue;

static inline void
print_handvalue(HandValue hv) {
	switch (hv) {
		case HIGH_CARD:
			printf("HIGH CARD\n");
			break;
		case ONE_PAIR:
			printf("ONE PAIR\n");
			break;
		case TWO_PAIR:
			printf("TWO PAIR\n");
			break;
		case THREE_OF_A_KIND:
			printf("THREE OF A KIND\n");
			break;
		case FULL_HOUSE:
			printf("FULL HOUSE\n");
			break;
		case FOUR_OF_A_KIND:
			printf("FOUR OF A KIND\n");
			break;
		case FIVE_OF_A_KIND:
			printf("FIVE OF A KIND\n");
			break;
	}
}

void print_hand(Hand hand);

static inline int
contains( int nums[5], int num) {
	int contains = 0, i = 0;
	for (; i<5; contains+=(nums[i++] == num));
	return contains;
}

static HandValue
handvalue( Hand *h ) {

	char kinds[5] = {0};
	int kind_count[5] = {0};

	int kinds_index = 0;

	for ( int i = 0; i<5; ++i ){
		int j = 0; int found = 0;
		for ( ; j < kinds_index; ++j )
			if ( kinds[j] == h->cards[i] ){
				found = 1;
				break;
			}
		if (found) kind_count[j]++;
		else { kinds[kinds_index] = h->cards[i];
		       kind_count[kinds_index]++;
			kinds_index++;
		     }
	}

	if (contains(kind_count, 5))
		return FIVE_OF_A_KIND;
	if (contains(kind_count, 4))
		return FOUR_OF_A_KIND;
	if (contains(kind_count, 3) && contains(kind_count, 2))
		return FULL_HOUSE;
	if (contains(kind_count, 3))
		return THREE_OF_A_KIND;
	if (contains(kind_count, 2) == 2)
		return TWO_PAIR;
	if (contains(kind_count, 2))
		return ONE_PAIR;
	return HIGH_CARD;
}

static HandValue
handvalue_pt2( Hand *h ) {

	char kinds[5] = {0};
	int kind_count[5] = {0};

	int kinds_index = 0;

	for ( int i = 0; i<5; ++i ){
		int j = 0; int found = 0;
		for ( ; j < kinds_index; ++j )
			if ( kinds[j] == h->cards[i] ){
				found = 1;
				break;
			}
		if (found) kind_count[j]++;
		else { kinds[kinds_index] = h->cards[i];
		       kind_count[kinds_index]++;
			kinds_index++;
		     }
	}

	// add the amount of jokers to the max number in the kind_count array
	int i = 0, found = 0;
	for (; i<5; ++i)
		if ( ( found = kinds[i] == 'J') )
			break;
	if (found) {
		int j_s = kind_count[i];
		kind_count[i] = 0;

		int max = 0; int j;
		for ( i = 0; i<5; ++i) {
			if (max < kind_count[i]) {
				max = kind_count[i];
				j = i;
			}
		};
		
		kind_count[j] += j_s;
	}

	if (contains(kind_count, 5))
		return FIVE_OF_A_KIND;
	if (contains(kind_count, 4))
		return FOUR_OF_A_KIND;
	if (contains(kind_count, 3) && contains(kind_count, 2))
		return FULL_HOUSE;
	if (contains(kind_count, 3))
		return THREE_OF_A_KIND;
	if (contains(kind_count, 2) == 2)
		return TWO_PAIR;
	if (contains(kind_count, 2))
		return ONE_PAIR;
	return HIGH_CARD;
}

static int
comparecards( Hand *h1, Hand *h2 ) {
	for (int i = 0; i < 5; ++i) {
		int i1 = isdigit(h1->cards[i]) ? h1->cards[i] - '0' : lettervalue(h1->cards[i]);
		int i2 = isdigit(h2->cards[i]) ? h2->cards[i] - '0' : lettervalue(h2->cards[i]);
		if ( i1 - i2 ) return i1 - i2;
	}
	return 0;
}

static int
comparecards_pt2( Hand *h1, Hand *h2 ) {
	for (int i = 0; i < 5; ++i) {
		int i1 = isdigit(h1->cards[i]) ? h1->cards[i] - '0' : lettervalue_pt2(h1->cards[i]);
		int i2 = isdigit(h2->cards[i]) ? h2->cards[i] - '0' : lettervalue_pt2(h2->cards[i]);
		if ( i1 - i2 ) return i1 - i2;
	}
	return 0;
}

static int
comparehands( const void *p1, const void *p2 ) {
	Hand *hand1 = (Hand *)p1;
	Hand *hand2 = (Hand *)p2;
	int handvalue_diff = handvalue(hand1) - handvalue(hand2);
	return ( !handvalue_diff ? comparecards(hand1, hand2) : handvalue_diff );
}

static int
comparehands_pt2( const void *p1, const void *p2 ) {
	Hand *hand1 = (Hand *)p1;
	Hand *hand2 = (Hand *)p2;
	int handvalue_diff = handvalue_pt2(hand1) - handvalue_pt2(hand2);
	return ( !handvalue_diff ? comparecards_pt2(hand1, hand2) : handvalue_diff );
}

Hand
parse_hand( char **line ) {
	Hand hand;
	for ( int i = 0; i < 5; hand.cards[i++] = *((*line)++) );
	hand.bid = eat_next_int(line);
	return hand;
}

void
print_hand(Hand hand){
	for (int i = 0; i<5; putc(hand.cards[i++], stdout));
	putc(' ', stdout);
	printf("%d\n", hand.bid);
}

void
part_1_solution( Hand hands[HANDS] ) {


	qsort(hands, HANDS, sizeof(Hand), comparehands);

	// for (int i = 0; i<HANDS; print_hand(hands[i++]));

	int acc = 0;
	for ( int i = 1; i<=HANDS; ++i) {
		acc += hands[i-1].bid * i;
	}
	printf("Part 1 solution: %d\n", acc);
}

void
part_2_solution( Hand hands[HANDS] ) {


	qsort(hands, HANDS, sizeof(Hand), comparehands_pt2);

	// for (int i = 0; i<HANDS; print_hand(hands[i]), print_handvalue(handvalue_pt2(&(hands[i++]))) );

	int acc = 0;
	for ( int i = 1; i<=HANDS; ++i) {
		acc += hands[i-1].bid * i;
	}
	printf("Part 2 solution: %d\n", acc);
}

int
main(int argc, char *argv[]){

	FILE *fp = fopen(FILENAME, "r");
	char *line = NULL;
	char *saveptr;
	size_t line_len;
	ssize_t read;

	Hand hands[HANDS];
	int hands_index = 0;

	while ( (read = getline(&line, &line_len, fp)) != -1 ) {
		
		saveptr = line;
		hands[hands_index++] = parse_hand(&line);
		line = saveptr;

	}
	
	// part_1_solution(hands);
	
	// for (int i = 0; i<HANDS; print_hand(hands[i++]));
	
	part_2_solution(hands);

	free(line);

	fclose(fp);

	return 0;
}
