#include <stdio.h>

int
score(int o, int u){
    int score = u-'W';
    if (u-o == 23) return score+3;
    if (o=='B'&&u=='Z'||
        o=='C'&&u=='X'||
        o=='A'&&u=='Y')
        return score+6;
    return score;
}

int
strategy(int o, int e){
    int score = ((e-'X')*3);
    if (e=='Y') return score+(o-'A'+1);
    if (e=='X'){
        if (o=='A') return score+3;
        if (o=='B') return score+1;
        if (o=='C') return score+2;
    }
    if (e=='Z'){
        if (o=='A') return score+2;
        if (o=='B') return score+3;
        if (o=='C') return score+1;
    }
}

int main(){
    FILE *f = fopen("i", "r");
    char o, u;
    int s = 0, t = 0;
    while (fscanf(f, "%c %c ", &o, &u)==2){
        s+=score(o, u);
        t+=strategy(o, u);
    }
    printf("%d %d\n", s, t);
    return 0;
}
