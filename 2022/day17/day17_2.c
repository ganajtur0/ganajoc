#include <stdio.h>

typedef
struct {
    int y;
    int x;
    int valid;
} pos;

typedef pos rock[5];

#define H 8192
#define W 7

void
rock_copy(rock from, rock to){
    for (int i = 0; i<5; i++)
        to[i] = from[i];
}

void
rock_print(rock r){
    for (int i = 0; i<5; i++){
        if (!r[i].valid)
            break;
        printf("y: %d; x: %d\n", r[i].y, r[i].x);
    }
}

void
rock_jet(char stage[H][W], rock r, char dir){
    int d = (dir == '>' ? 1 : -1);
    rock tmp;
    rock_copy(r, tmp);
    for (int i = 0; i<5; i++){
        if (!tmp[i].valid)
            break;
        tmp[i].x += d;
        if (tmp[i].x < 0 || tmp[i].x > 6 || stage[tmp[i].y][tmp[i].x] == '#')
            return;
    }
    rock_copy(tmp, r);
}

void
rock_set_y(rock r, rock original, int v){
    for (int i = 0; i<5; i++){
        if (!original[i].valid)
            break;
        r[i].y = v-original[i].y;
    }
}

// 1 if stopped 0 if not
int
rock_fall(char stage[H][W], rock r){
    rock tmp;
    rock_copy(r, tmp);
    for (int i = 0; i<5; i++){
        if (!tmp[i].valid)
            break;
        tmp[i].y++;
        if (tmp[i].y > (H-1) || stage[tmp[i].y][tmp[i].x] == '#' )
            return 1;
    } 
    rock_copy(tmp, r);
    return 0;
}

void
rock_stop(char stage[H][W], rock r, int *top){
    (*top)+=4;
    for (int i = 0; i<5; i++){
        if (!r[i].valid)
            break;
        if (r[i].y <= *top)
            *top = r[i].y;
        stage[r[i].y][r[i].x] = '#';
    }
}

void
stage_print(char stage[H][W], int dist){
    for (int i = (H-dist); i<H; i++){
        for (int j = 0; j<W; j++)
            putc(stage[i][j], stdout);
        putc('\n', stdout);
    }
    putc('\n', stdout);
}

int
main(){

#ifdef TEST
    FILE *f = fopen("test", "r");
#else //TEST
    FILE *f = fopen("i", "r");
#endif //TEST
    
    char stage[H][W];
    for (int i = 0; i<H; i++)
        for (int j = 0; j<W; j++)
            stage[i][j] = '.';

    char buf[16384];
    fgets(buf, 16384, f);

    char *i;
    for (i=buf; *i!='\n'&&*i!='\0'; i++);
    *i='\0';

    rock rocks[5] = {
        // distance from bottom; distance from left edge; necessity of that coordinate
        { {0,2,1},{0,3,1},{0,4,1},{0,5,1},{0,0,0} }, // -
        { {2,3,1},{1,2,1},{1,3,1},{1,4,1},{0,3,1} }, // +
        { {2,4,1},{1,4,1},{0,2,1},{0,3,1},{0,4,1} }, // L
        { {0,2,1},{1,2,1},{2,2,1},{3,2,1},{0,0,0} }, // I
        { {0,2,1},{0,3,1},{1,2,1},{1,3,1},{0,0,0} }, // O
    }; 

    int top = H, r_idx = 0, stopped = 0;
    rock tmp;
    i = buf;
    for (int j = 0; j<2022; j++){

        top-=4;
        rock_copy(rocks[r_idx], tmp);
        rock_set_y(tmp, rocks[r_idx], top);

        while (!stopped){

            if (*i == '\0')
                i = buf;
            rock_jet(stage, tmp, *i);
            stopped = rock_fall(stage, tmp);
            i++;

        }
        
        rock_stop(stage, tmp, &top);
        r_idx=( (r_idx+1)%5 );
        stopped = 0;
        // stage_print(stage, 20);
        // printf("%d\n", H-top);


    }

    printf("%d\n", 69420);

    return 0;
}
