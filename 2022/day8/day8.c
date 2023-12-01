#include <stdio.h>

#ifdef TEST
#define X 5
#else
#define X 99
#endif //TEST

int
scenic_score(char trees[X][X], char y, char x){
    int score=1;
    int i;
    for (i=1; y-i>0 && trees[y-i][x]<trees[y][x]; i++);
    score*=i;
    for (i=1; y+i<X-1 && trees[y+i][x]<trees[y][x]; i++);
    score*=i;
    for (i=1; x-i>0 && trees[y][x-i]<trees[y][x]; i++);
    score*=i;
    for (i=1; x+i<X-1 && trees[y][x+i]<trees[y][x]; i++);
    score*=i;
#ifdef DEBUG
    printf("scenic score of %d %d: %d\n", y, x, score);
#endif //DEBUG
    return score;
}

void
neighbors(char trees[X][X], char n[4], char y, char x){
    // n = {top, bottom, left, right}
    int i;
    for (i=y-1; i>0 && trees[i][x]<trees[y][x]; i--);
    n[0]=trees[i][x];
    for (i=y+1; i<X-1 && trees[i][x]<trees[y][x]; i++);
    n[1]=trees[i][x];
    for (i=x-1; i>0 && trees[y][i]<trees[y][x]; i--);
    n[2]=trees[y][i];
    for (i=x+1; i<X-1 && trees[y][i]<trees[y][x]; i++);
    n[3]=trees[y][i];

#ifdef DEBUG
    printf(" %d \n%d%d%d\n %d \n", n[0], n[2], trees[y][x], n[3], n[1]);
#endif //DEBUG
}

int
is_visible(char tree, char n[4]){
    for (int i = 0;i<4;i++)
        if (tree>n[i])
            return 1;
    return 0;
}

void
str_strip(char *str){
    char *c;
    for (c = str; *c!='\n' && *c!='\0';c++);
    *c = '\0';
}

int
main(void){
#ifdef TEST
    FILE *f = fopen("test","r");
#else
    FILE *f = fopen("i","r");
#endif
    char trees[X][X];
    char line[X+2], n[4];
    int y=0, tree_count=4*X-4;
    while (fgets(line, X+2, f)!=(void*)(0)){
        str_strip(line);
        for (int x = 0; line[x]!='\0'; x++)
            trees[y][x] = line[x]-'0';
        y++;
    }
#ifdef TEST
    neighbors(trees, n, 1, 3);
    neighbors(trees, n, 3, 3);
    scenic_score(trees, 3, 2);
#endif //TEST
    int max_score = 0;
    for (int i = 1; i<X-1; i++)
        for (int j = 1; j<X-1; j++){
            int s = scenic_score(trees, i, j);
            max_score = (s>max_score ? s : max_score);
            neighbors(trees, n, i, j);
            tree_count += is_visible(trees[i][j], n);
        }
    printf("%d %d\n", tree_count, max_score);
    fclose(f);
    return 0;
}
