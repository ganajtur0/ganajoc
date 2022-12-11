// the solution to the second part is written as well,
// but it does not wirk properly
// I cannot be bothered to figure out where the integer
// overflows are happening, so I cheated with python
// sorry

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define ISNUM(X) ('0'<=X && X<='9')

typedef
enum {
    ADD,
    MUL,
    SUB,
} op_enum;

typedef
struct {
    int a;
    int is_self;
    op_enum o;
} monke_func;

typedef
struct {
    int arr[50];
    int count;
} stack;

typedef
struct {
    stack items;
    monke_func f;
    int test;
    int true_idx;
    int false_idx;
    int total;
} monke;

void
stack_push(stack *s, int val){
    s->arr[(s->count)++] = val;
}

void
stack_print(stack s){
    for (int i = 0; i<s.count; i++)
        printf("%d, ", s.arr[i]);
    putc('\n', stdout);
}

int
stack_pop(stack *s){
    return s->arr[--(s->count)];
}

int
stack_pop_front(stack *s){
    int tmp = s->arr[0];
    for (int i = 1; i<(s->count); i++)
        s->arr[i-1] = s->arr[i];
    (s->count)--;
    return tmp;
}

void
rm_indent(char *s){
    int i = 0, l = strlen(s);
    for (char *j = s; *j==' '; j++, i++);
    for (int j = 0; j<l-i+1; j++)
        s[j] = s[j+i];
}

char
str_len(char *str){
    int len = 0;
    for (char *iter = str;
         *iter != '\0' && *iter != ',' && *iter != '\n';
         iter++, len++);
    return len;
}

int tenpow(int p){
    int r = 1;
    for (r = 1; p>0; r*=10, p--);
    return r;
}

int
parse_item(char *item_str){
    int parsed = 0;
    char len = str_len(item_str)-1;
    for (char *iter = item_str;
         *iter != '\0' && *iter != ',' && *iter != '\n';
         iter++, len--)
        parsed += ((*iter)-'0')*tenpow(len);
    return parsed;
}

void
parse_monke_func(monke *m, char *s){
    char *token = strtok(s, " ");
    int state = 0;
    do {
        switch (state){
            case 0:
                switch (token[0]){
                    case '*':
                        m->f.o = MUL;
                        state++;
                        break;
                    case '+':
                        m->f.o = ADD;
                        state++;
                        break;
                    default:
                        break;
                }
                break;
            case 1:
                if (token[0]=='o'){
                    m->f.is_self = 1;
                    break;
                } else {
                    m->f.is_self = 0;
                    m->f.a = parse_item(token);
                    break;
                }
                break;
        }
        token = strtok(NULL, " ");
    } while(token!=NULL);
}

void
parse_items(monke *m, char *s){
    char *token = strtok(s, " ");
    do {
        if (ISNUM(token[0])){
            stack_push(&(m->items), parse_item(token));
        }
        token = strtok(NULL, " ");
    } while(token!=NULL);
}

void
parse_test(monke *m, char *s){
    char *token = strtok(s, " ");
    do {
        if (ISNUM(token[0])){
            m->test = parse_item(token);
        }
        token = strtok(NULL, " ");
    } while(token!=NULL);
}

void
parse_branching(monke *m, char *s){
    char *token = strtok(s, " ");
    int is_true = 0;
    do {
        if (strncmp(token, "true:", 5)==0){
            is_true = 1;
        } else if (ISNUM(token[0])){
            if (is_true)
                m->true_idx = parse_item(token);
            else
                m->false_idx = parse_item(token);
            break;
        }
        token = strtok(NULL, " ");
    } while(token!=NULL);
}

int
monke_exec(int old, monke_func f){
    int a = f.is_self ? old : f.a;
    switch(f.o){
        case ADD:
            return old+a;
        case MUL:
            return old*a;
        case SUB:
            return old-a;
        // unreachable
        default:
            return 0;
    }
}

void
monke_round(monke monkes[10], int n){
    for (int i = 0; i<n; i++){
        int item_count = monkes[i].items.count;
        monkes[i].total+=item_count;
        for (int j = 0; j<item_count; j++){
            int item = stack_pop_front(&(monkes[i].items));
            item = monke_exec(item, monkes[i].f)/3;
            if (!(item%monkes[i].test)){
                stack_push( &(monkes[monkes[i].true_idx].items), item );
            }
            else{
                stack_push( &(monkes[monkes[i].false_idx].items), item );
            }
        }
    }
}

void
monke_round_two(monke monkes[10], int n, int mod){
    for (int i = 0; i<n; i++){
        int item_count = monkes[i].items.count;
        monkes[i].total+=item_count;
        for (int j = 0; j<item_count; j++){
            int item = stack_pop_front(&(monkes[i].items));
            item = monke_exec(item, monkes[i].f)%mod;
            if (!(item%monkes[i].test)){
                stack_push( &(monkes[monkes[i].true_idx].items), item );
            }
            else{
                stack_push( &(monkes[monkes[i].false_idx].items), item );
            }
        }
    }
}

int
main(){
#ifdef TEST
    FILE *f = fopen("test", "r");
#else
    FILE *f = fopen("i", "r");
#endif

    monke monkes[10];
    int monke_num = 0;

/*
    printf("stack_test\n");
    stack s;
    s.count = 0;
    stack_push(&s, 34);
    stack_push(&s, 35);
    printf("stack_length: %d\n", s.count);
    stack_print(s);
    int a = stack_pop_front(&s);
    printf("%d\n", a);
    stack_print(s);
    int b = stack_pop_front(&s);
    printf("%d\n", b);
    stack_print(s);
    int res = a+b;
    printf("result: %d, stack length: %d\n", res, s.count);
*/
    char line[512];
    while (fgets(line, 512, f)!=NULL){
        if (line[0]=='M'){
            stack s;
            s.count = 0;
            monke_func f;
            monke m;
            m.items = s;
            m.f = f;
            m.test = 0;
            m.true_idx = 0;
            m.false_idx = 0;
            monkes[monke_num++] = m;
        }
        rm_indent(line);
        switch (line[0]){
            case 'S':
                parse_items(&(monkes[monke_num-1]), line);
                break;
            case 'O':
                parse_monke_func(&(monkes[monke_num-1]), line);
                break;
            case 'T':
                parse_test(&(monkes[monke_num-1]), line);
                break;
            case 'I':
                parse_branching(&(monkes[monke_num-1]), line);
                break;
            default:
                break;
        }

    }

    int i, first=0, second=0;
    for (i = 0; i<20; i++)
        monke_round(monkes, monke_num);
    for (i = 0; i<monke_num; i++){
        if (first<monkes[i].total){
            second = first;
            first = monkes[i].total;
        }
        else if (second<monkes[i].total)
            second = monkes[i].total;
    }
    printf("%d ", first*second);
    int mod=1;
    for (int i = 0; i<monke_num; i++)
        mod*=monkes[i].test;

    i, first=0, second=0;
    for (i = 0; i<10000; i++)
        monke_round_two(monkes, monke_num, mod);
    for (i = 0; i<monke_num; i++){
        if (first<monkes[i].total){
            second = first;
            first = monkes[i].total;
        }
        else if (second<monkes[i].total)
            second = monkes[i].total;
    }
    printf("%ld\n", first*second);
	return 0;
}
