#include <stdio.h>

#ifdef TEST
    #define X 3
    #define Y 3
#else
    #define X 9
    #define Y 8
#endif //TEST

typedef
struct {
    int n;
    int from;
    int to;
} move;

void
add_crates(char *s, int r, char c[X][X*Y]){
    for (int i=0; s[i]!='\0';i++)
        if (s[i]>='A'&&s[i]<='Z')
            c[i/4][r]=s[i];
}

int
top_crate(char c[X*Y]){
    for (int i=0;i<X*Y;i++)
        if(c[i]==0) return i;
}

void
move_crates(char c[X][X*Y], move m){
    int i_to = top_crate(c[m.to-1]);
    int i_from = top_crate(c[m.from-1])-1;
    for (int i=0;i<m.n;i++){
        c[m.to-1][i_to] = c[m.from-1][i_from];
        c[m.from-1][i_from] = 0;
        i_to++; i_from--;
    }
}

void
move_crates_9001(char c[X][X*Y], move m){
    int i_to = top_crate(c[m.to-1]);
    int i_from = top_crate(c[m.from-1])-m.n;
    for (int i=0;i<m.n;i++){
        c[m.to-1][i_to] = c[m.from-1][i_from];
        c[m.from-1][i_from] = 0;
        i_to++; i_from++;
    }
}

void
print_crates(char c[X][X*Y]){
    for (int i=0;i<X;i++){
        for (int j=0;j<X*Y;j++)
            if (c[i][j]==0)
                printf("x ");
            else
                printf("%c ", c[i][j]);
        putc('\n', stdout);
    }
}

int main(){
#ifdef TEST
    FILE *f = fopen("test", "r");
#else
    FILE *f = fopen("i", "r");
#endif //TEST
    char row[128];
    char crates[X][X*Y]={0};
    char crates_2[X][X*Y]={0};
    for (int i=0;i<Y;i++){
        fgets(row, 128, f);
        add_crates(row, Y-i-1, crates);
        add_crates(row, Y-i-1, crates_2);
    }
    // skip two rows
    for (int i=0; i<2; i++) fgets(row, 128, f);
    move m;
    while (fscanf(f, "move %d from %d to %d ", &m.n, &m.from, &m.to) == 3){
        move_crates(crates, m);
        move_crates_9001(crates_2, m);
    }
    for (int i=0;i<X;i++)
        putc(crates[i][top_crate(crates[i])-1],stdout);
    putc(' ', stdout);
    for (int i=0;i<X;i++)
        putc(crates_2[i][top_crate(crates_2[i])-1],stdout);
    putc('\n',stdout);
    fclose(f);
    return 0;
}
