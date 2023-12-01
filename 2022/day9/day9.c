#include <stdio.h>
#ifdef DEBUG
#include <unistd.h>
#endif //DEBUG

#define abs(X) (X<0 ? X*-1 : X)

#define MIN(X, Y) (X<Y ? X : Y)
#define MAX(X, Y) (X>Y ? X : Y)

#define POS_EQ(P, Q) (P.x == Q.x && P.y == Q.y)

#define pos_diagonal(P) (abs(P.x)==1 && abs(P.y)==1)

#define POS_ADD(P, Q) do {\
    P->x+=Q.x;            \
    P->y+=Q.y;            \
}while(0)                 \


#define print_pos(P) do {\
    printf("%d %d\n", P.x, P.y);\
}while(0)


typedef
struct {
    int x;
    int y;
} pos;

#ifdef DEBUG
    #ifdef TEST
        #define X 6
        #define Y 5
    #else
        #define X 100
        #define Y 50
    #endif //TEST
char map[Y][X];
void
print_map(pos h, pos t){
        printf("\e[1;1H\e[2J");
       for (int i = 0; i<Y; i++)
           for (int j = 0; j<X; j++)
               map[i][j] = '.';
       map[(Y-1)-t.y][t.x] = 'T';
       map[(Y-1)-h.y][h.x] = 'H';
       for (int i = 0; i<Y; i++){
           for (int j = 0; j<X; j++)
               putc(map[i][j], stdout);
           putc('\n', stdout);
       }
       char halt;
       scanf("%c", &halt);
       /*
       usleep(500000);
       */
}
void
print_rope_map(pos rope[10]){
        printf("\e[1;1H\e[2J");
        int i, j;
        for (i = 0; i<10; i++)
            printf("%d %d\n", rope[i].x, rope[i].y);
       for (i = 0; i<Y; i++)
           for (j = 0; j<X; j++)
               map[i][j] = '.';
       for (i = 0; i<10; i++)
            map[(Y-1)-rope[i].y][rope[i].x] = '0'+i;
       for (i = 0; i<Y; i++){
           for (j = 0; j<X; j++)
               putc(map[i][j], stdout);
           putc('\n', stdout);
       }
       char halt;
       scanf("%c", &halt);
       /*
       usleep(500000);
       */
}
#endif

typedef
struct {
    // Top 5 BRUH momentos
    pos arr[10000];
    int count;
} pos_arr;

int
pos_in_arr(const pos_arr a, const pos p){
    for (int i=0; i<a.count; i++){
        if (POS_EQ(a.arr[i], p))
            return 1;
    }
    return 0;
}

void
pos_append(pos_arr *a, pos p){
    (a->arr)[(a->count)++] = p;
}

pos
pos_sub(pos p, pos q){
    pos sub;
    sub.x = p.x-q.x;
    sub.y = p.y-q.y;
    return sub;
}

void
pos_update(pos *h, pos *t, char d, int steps, pos_arr *a){
#ifdef DEBUG
    printf("\n%c%d:\n", d, steps);
#endif
    pos dir = {0,0};
    switch (d){
        case 'R':
            dir.x = 1;
            break;
        case 'L':
            dir.x = -1;
            break;
        case 'U':
            dir.y = 1;
            break;
        case 'D':
            dir.y = -1;
            break;
    }

    for (int i = 0; i<steps; i++){
        POS_ADD(h, dir);
        pos tmp = pos_sub(*h, *t);
        pos diff = pos_sub(*h,*t);
        diff.x = MIN(1, MAX(-1, diff.x));
        diff.y = MIN(1, MAX(-1, diff.y));
        if (!POS_EQ(tmp, diff))
            POS_ADD(t, diff);
#ifdef DEBUG
        print_map(*h, *t);
#endif
        if (!pos_in_arr(*a, *t))
            pos_append(a, *t);
    }
}


void
rope_update(pos rope[10], char d, int steps, pos_arr *a){
#ifdef DEBUG
    printf("\n%c%d:\n", d, steps);
#endif
    pos dir = {0,0};
    switch (d){
        case 'R':
            dir.x = 1;
            break;
        case 'L':
            dir.x = -1;
            break;
        case 'U':
            dir.y = 1;
            break;
        case 'D':
            dir.y = -1;
            break;
    }

    pos *h, *t;
    for (int i = 0; i<steps; i++){
        h = &(rope[0]);
        POS_ADD(h, dir);
        for (int j = 0; j<9; j++){

            h = &(rope[j]);
            t = &(rope[j+1]);

            pos tmp = pos_sub(*h, *t);
            pos diff = pos_sub(*h,*t);
            diff.x = MIN(1, MAX(-1, diff.x));
            diff.y = MIN(1, MAX(-1, diff.y));
            if (!POS_EQ(tmp, diff))
                POS_ADD(t, diff);
        }
#ifdef DEBUG
        print_rope_map(rope);
#endif
        if (!pos_in_arr(*a, *t))
            pos_append(a, *t);
    }
}
int
main(){

#ifdef TEST
    FILE *f = fopen("test", "r");
#else
    FILE *f = fopen("i", "r");
#endif //TEST

#if defined(DEBUG) && !defined(TEST)
    pos h = {50, 25};
    pos t = {50, 25};
#else
    pos h = {0, 0};
    pos t = {0, 0};
#endif
    pos_arr a;
    pos_arr b;
    a.count = 0;
    b.count = 0;
    pos_append(&a, h);

    char d; int s;
    while (fscanf(f, "%c %d ", &d, &s)==2)
        pos_update(&h, &t, d, s, &a);
    printf("%d ", a.count);

    rewind(f);
    pos rope[10];
    for (int i = 0; i<10; i++){
        pos p = {0,0};
        rope[i] = p;
    }
    while (fscanf(f, "%c %d ", &d, &s)==2)
        rope_update(rope, d, s, &b);
    printf("%d\n", b.count);

    fclose(f);
    return 0;
}
