#include <stdio.h>

void
update_sprite(char sprite[41], int pos){
    int i;
    for (i=0; i<40; i++)
        sprite[i] = '.';
    for (i=-1; i<2; i++) sprite[pos+i] = '#';
}

int
main(){
#ifdef TEST
    FILE *f = fopen("test", "r");
#else
    FILE *f = fopen("i", "r");
#endif //TEST

    int x = 1, res, cycle = 1, v, c = 20, sum = 0;
    char inst[5], sprite[41];
    sprite[40] = '\0';
    update_sprite(sprite, x);

    while ( (res=fscanf(f, "%s %d ", inst, &v)) >= 1 ){
        if (res == 2){
            cycle++;
            if (cycle == c){
                printf("%d %d\n", c, x);
                sum+=c*x;
                c+=40;
            }
            x+=v;

            cycle++;
            if (cycle == c){
                sum+=c*x;
                c+=40;
            }
        }
        else{
            cycle++;
            if (cycle == c){
                sum+=x*c;
                c+=40;
            }
        }
    }
    printf("%d\n", sum);

    cycle = 0; x = 1;
    rewind(f);
    putc(sprite[0], stdout);
    while ( (res=fscanf(f, "%s %d ", inst, &v)) >= 1 ){
        if (res == 2){
            cycle=((++cycle)%40);
            if (!cycle) putc('\n', stdout);
            putc(sprite[cycle],stdout);
            x+=v;
            update_sprite(sprite, x);
            cycle=((++cycle)%40);
            if (!cycle) putc('\n', stdout);
            putc(sprite[cycle],stdout);
        }
        else{
            cycle=((++cycle)%40);
            if (!cycle) putc('\n', stdout);
            putc(sprite[cycle],stdout);
        }
    }
    putc('\n', stdout);
    fclose(f);
    return 0;
}
