#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// this was a low blow on day11 :(
// big loss for the based C community
#include <gmp.h>

#define BUFFER_SIZE 40

/*
int
n_digits( n) {
	int i = n;
	int j = 0;
	do {
		i /= 10;
		j++;
	} while (i > 0);
	return j;
}
*/

typedef struct llt {
	// unsigned long long value;
	mpz_t value;
	struct llt *next;
	struct llt *prev;
} ll_t;

ll_t*
ll_insert ( ll_t *l, char buffer[BUFFER_SIZE/2] ) {
	ll_t *nl = malloc(sizeof(ll_t));
	mpz_init(nl->value);
	// nl->value = x;
	mpz_set_str(nl->value, buffer, 10);
	nl->prev  = l;
	nl->next  = l->next;
	l->next   = nl;
	return nl;
}

ll_t *
ll_add ( ll_t *l, unsigned long x, int count) {
	if (l->prev == NULL && l->next == NULL && count == 0){
		// l->value = x;
		mpz_init(l->value);
		mpz_set_ui(l->value, (unsigned long)x);
		return l;
	}
	ll_t *nl = malloc(sizeof(ll_t));
	mpz_init(nl->value);
	mpz_set_ui(nl->value, (unsigned long)x);
	nl->prev = l;
	nl->next = l->next;
	l->next  = nl;
	return nl;
}

void
ll_print( ll_t *l ) {
	ll_t *iter = l;
	char buffer[BUFFER_SIZE];
	while (iter != NULL) {
		printf("%s ", mpz_get_str(buffer, 10, iter->value));
		iter = iter->next;
	}
	putchar('\n');
}

int
ll_len ( ll_t *l ) {
	int n = 0;
	ll_t * iter = l;
	while (iter != NULL) {
		n++;
		iter = iter->next;
	}
	return n;
}

int
part1( ll_t *l ) {

	ll_t *iter = l;
	char conv_buffer[BUFFER_SIZE] = {0};
	char num1[BUFFER_SIZE/2] = {0};
	char num2[BUFFER_SIZE/2] = {0};


	for ( int i = 0; i<5; i++) {

		while (iter != NULL) {

			memset(conv_buffer, 0, 40);
			mpz_get_str(conv_buffer, 10, iter->value);
			int digits = 0;
			while ( conv_buffer[digits] != '\0' ) digits++;

			// int digits = n_digits(iter->value);
			if ( digits % 2 == 0 ) {

				// memset(conv_buffer, 0, 40);
				memset(num1, 0, BUFFER_SIZE/2);
				memset(num2, 0, BUFFER_SIZE/2);

				// snprintf(conv_buffer, 40, "%llu", iter->value);
				memcpy(num1, conv_buffer, digits/2);
				memcpy(num2, &(conv_buffer[digits/2]), digits/2);

				// unsigned long long val1 = atoll(num1);
				// unsigned long long val2 = atoll(num2);
				
				mpz_set_str(iter->value, num1, 10);
				iter = ll_insert(iter, num2);
			}
			else if ( mpz_get_ui(iter->value) == 0 ) {
				mpz_set_ui(iter->value, 1);
			}
			else {
				mpz_mul_ui(iter->value, iter->value, 2024);
			}
			iter = iter->next;

			
		}

		ll_print(l);
		putchar('\n');
		iter = l;
	}

	return ll_len(l);
}

int
main(int argc, char *argv[]){

	FILE *fp;

	#ifdef DEBUG
	fp = fopen("test", "r");
	#else
	fp = fopen("i", "r");
	#endif

	if ( fp == NULL ) return 1;

	ll_t l;
	l.next  = NULL;
	l.prev  = NULL;

	unsigned long i;
	int count = 0;
	ll_t *iter = &l;

	while ( fscanf(fp, "%lu", &i) != EOF ) {
		iter = ll_add(iter, i, count);
		count++;
	};

	// ll_print(&l);

	printf("PART 1 : %d\n", part1(&l));
/*
	while ( (fscanf(fp, "%d", &(da.a[(da.n)++]) ) ) != EOF );
	da.n--;
*/

	return 0;
}
