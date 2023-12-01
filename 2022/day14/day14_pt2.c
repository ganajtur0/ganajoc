#include<stdio.h>
#include<string.h>
#include<stdlib.h>
// #include<unistd.h>

#define W 1500
#define H 1500

#define MAX(X, Y) (X > Y ? X : Y)
#define MIN(X, Y) (X < Y ? X : Y)

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
parse_num(char *num_str){
    int parsed = 0;
    char len = str_len(num_str)-1;
    for (char *iter = num_str;
         *iter != '\0' && *iter != ',' && *iter != '\n';
         iter++, len--)
        parsed += ((*iter)-'0')*tenpow(len);
    return parsed;
}

void
display_map(char map[H][W]){
    for (int i = 0; i<H; i++){
        for (int j = 0; j<W; j++){
            putc(map[i][j], stdout);
        }
        putc('\n', stdout);
    }
}

void
parse_coordinates(char *s, int *x, int *y){
    *x = parse_num(s);
    char *i;
    for (i = s; *i!=','; i++);
    i++;
    *y = parse_num(i);
}

void
add_stone(char map[H][W], int s1, int s2, int e1, int e2){
    // s1-=400; e1-=400;
    for (int i = MIN(s2, e2); i <= MAX(s2, e2); i++)
        for (int j = MIN(s1, e1); j <= MAX(s1, e1); j++)
            map[i][j] = '#';
}

void
parse_stone(char map[H][W], char *s, int *maxy){
    int x1, y1, x2, y2;
    char *token = strtok(s, " -> ");
    parse_coordinates(token, &x1, &y1);
    *maxy = MAX((*maxy), y1);
    for (;;){
        token = strtok(NULL, " -> ");
        if (token == NULL)
            break;
        parse_coordinates(token, &x2, &y2);
        *maxy = MAX((*maxy), y2);
        add_stone(map, x1, y1, x2, y2);
        x1 = x2;
        y1 = y2;
    }
}

int
sand_sim(char map[H][W]){
    int i=-1, j=500;
    for (; i<H-1; i++){

        if (map[i+1][j] == 'o' || map[i+1][j] == '#'){

            // source is blocked
            if (i == -1)
                return 0;

            // checking left side
            if (j>0 && map[i+1][j-1] == '.'){
                j-=1;
                continue;
            }
            // checking right side
            if (j<(W-1) && map[i+1][j+1] == '.'){
                j+=1;
                continue;
            }

            map[i][j] = 'o';
            return 1;
        }

    }
}

int
main(){

    char map[H][W];
    int i;

    for (i = 0; i<H; i++){
        for (int j = 0; j<W; j++){
            map[i][j] = '.';
        }
    }

#ifdef TEST
    FILE *f = fopen("test", "r");
#else
    FILE *f = fopen("i", "r");
#endif //TEST

    char buf[1024];
    int maxy;

    while (fgets(buf, 1024, f)!=NULL)
        parse_stone(map, buf, &maxy);

    fclose(f);

    maxy+=2;

    for (i = 0; i<W; i++)
        map[maxy][i] = '#';

    int unit = 0;
    while (sand_sim(map)){
        unit++;
        //printf("\e[1;1H\e[2J");
        // sand_sim(map);
        // display_map(map);
        // putc('\n', stdout);
        // sleep(1);
    }

    printf("%d\n", unit);

	return 0;
}
