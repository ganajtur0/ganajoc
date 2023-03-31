#include<stdio.h>
#include<stdlib.h>

#define IS_DIGIT(X) (X<='9' && X>='0')

// going to parse the lists

typedef
enum {
	UNDEF,
	INT,
	LIST,
} Type;

typedef
enum {
	NONE,
	TRUE,
	FALSE,
} Compare;

typedef
struct obj {
	Type type;
	union {

	int i;
	struct {
		struct obj **ele;
		size_t len;
	} l;

	};
} obj;

obj *
new_object(Type type){
	size_t size = sizeof(obj);
	obj *p = malloc(size);
	if (!p) {
		fprintf(stderr, "Out of memory allocating %zu bytes\n", size);
		exit(1);
	}
	p->type = type;
	return p;
}

void
obj_array_realloc(obj *o) {
	o->l.ele = realloc(o->l.ele, sizeof(*o)*(o->l.len+1) );
	if (!o->l.ele) {
		fprintf(stderr, "Out of memory reallocating bytes\n");
		exit(1);
	}
}

obj *
parse_object(const char *s, const char **next){

	if (*s == ',')
		s++;

	obj *o = new_object(UNDEF);

	// integers
	if (IS_DIGIT(*s)) {
		char buf[5]; char *iter = buf;
		*iter = *s;
		s++;
		while (IS_DIGIT(*s)){
			*(++iter) = *s;
			s++;
		}
		*(++iter) = '\0';
		o->type = INT;
		o->i = atoi(buf);
		if (next) *next = s;
	}

	// lists
	else if (*s == '[') {

		o->type = LIST;
		o->l.len = 0;
		o->l.ele = NULL;
		
		s++;

		for (;;) {
			if (*s == ']') {
				if (next) *next = s+1;
				return o;
			}
			const char *nextptr;
			obj *element = parse_object(s, &nextptr);
			obj_array_realloc(o);
			o->l.ele[o->l.len++] = element;
			s = nextptr;

			continue;
		}
	}

	s++;
	return o;
}

void
release_object(obj *o){
	if (o->type == LIST){
		for (int i = 0; i<o->l.len; i++)
			release_object(o->l.ele[i]);
	}
	free(o);
}

// returns true if object 'a' "comes before" object 'b'
Compare
compare_objects(obj *a, obj *b) {
	if ( a->type == b->type ){
		switch (a->type){
		case LIST:
#ifdef TEST
			printf("Comparing LISTs\n");
#endif //TEST
			if (a->l.len == 0 && b->l.len > 0)
				return TRUE;
			else if (b->l.len == 0 && a->l.len > 0)
				return FALSE;
			for (int i = 0; i<a->l.len; i++){
				// list 'a' is longer than list 'b'
				if (i == b->l.len)
					return FALSE;
				Compare result = compare_objects(a->l.ele[i], b->l.ele[i]);
				if (result == NONE)
					continue;
				return result;
			}
			// this can happen in this case for example:
			// [ 4, 2, 0 ] compared to [ 4, 2, 0, 6, 9 ]
			return TRUE;
		case INT:
#ifdef TEST
			printf("Comparing INTs: %d < %d ?\n", a->i, b->i);
#endif //TEST
			if (a->i == b->i)
				return NONE;
			return a->i < b->i ? TRUE : FALSE;
		default:
			// unreachable
			return FALSE;

		}
	}		
	else if (a->type == LIST && b->type == INT){
#ifdef TEST
		printf("Compare LIST to INT\n");
#endif //TEST
		if (a->l.len == 0) return TRUE;
		if (a->l.ele[0]->i == b->i)
			return a->l.len > 1 ? FALSE : TRUE;
		return compare_objects(a->l.ele[0], b);

	}
	else if (a->type == INT  && b->type == LIST){
#ifdef TEST
		printf("Compare INT to LIST\n");
#endif //TEST
		if (b->l.len == 0) return TRUE;
		if (b->l.ele[0]->i == a->i)
			return b->l.len > 1 ? TRUE : FALSE;
		return compare_objects(a, b->l.ele[0]);
	}
}

void
print_object(obj *o){
	if (o->type == LIST){
		putc('[', stdout);
		for (int i = 0; i<o->l.len; i++){
			print_object(o->l.ele[i]);
		}
		putc(']', stdout);
	}
	else {
		printf("%d", o->i);
	}
	putc(',', stdout);
}

int
main(){

#ifdef TEST
    FILE *f = fopen("test.txt", "r");
#else
    FILE *f = fopen("i", "r");
#endif //TEST
    char pair[2][512];
    // i -> párosítás; j -> indexelés
    int i=0, j=1, sum=0;
    while (fgets(pair[i], 512, f)){
        if (pair[i][0] == '\n')
            i=-1;
        if (i == 1){
            i = 0;
	
#ifdef TEST
	    printf("Comparing:\n");
            printf("%s", pair[0]);
            printf("%s", pair[1]);
#endif //TEST

	    obj *pair_1 = parse_object(pair[0], NULL);
	    obj *pair_2 = parse_object(pair[1], NULL);

	    if( compare_objects(pair_1, pair_2) == TRUE ){
#ifdef TEST
		printf("TRUE\n");
#endif //TEST
		printf("%d ", j);
		sum+=j;
	    }
#ifdef TEST
	    else {
		printf("FALSE\n");
	    }
#endif //TEST

	    release_object(pair_1);
	    release_object(pair_2);
	
	    j++;
        }
        i++;
    }

    printf("\n%d\n", sum);

    fclose(f);
    return 0;
}
