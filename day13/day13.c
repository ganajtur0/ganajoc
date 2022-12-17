#include<stdio.h>

#define ISNUM(X) (X<='9' && X>='0')

int
right_order(char *tp, char *bt, int *idx_i, int *idx_j){
    (*idx_i)++; (*idx_j)++;
    for (int *i=idx_i, *j=idx_j; tp[*i]!='\n'&&bt[*j]!='\n'; (*i)++, (*j)++){

        printf("%c %c\n", tp[*i], bt[*j]);


        // mismatch top
        if (tp[*i]=='['){

            // mismatched types
            if ( bt[*j]!='[')
                (*j)--;

            // printf("recursion\n");
            // recursing
            return right_order(tp, bt, i, j);
                // return 0;
        }

        // mismatch bottom
        else if (bt[*j]=='['){

            // mismatched types
            if ( tp[*i]!='[')
                (*i)--;

            // printf("recursion\n");
            // recursing
            return right_order(tp, bt, i, j);
                // return0;
        }

        // comparing numbers
        else if ( ISNUM( (tp[*i]) ) ){
            // printf("comparing numbers\n");
            if ( bt[*j]-tp[*i] < 0 )
                // printf("wrong order\n");
                return 0;
            else if ( bt[*j]-tp[*i] > 0 )
                return 1;
        }

        else if (bt[*j]==']'){
            // printf("out of items\n");
            return 0;
        }


        /*else if (tp[*i] == ']' || bt[*j] == ']')
            // printf("return\n");
            return 1;*/
    }
    return 1;
}

int
main(){

#ifdef TEST
    FILE *f = fopen("test", "r");
#else
    FILE *f = fopen("i", "r");
#endif //TEST
    char pair[2][512];
    int i=0, sum=0, j=1;
    while (fgets(pair[i], 256, f)){
        if (pair[i][0] == '\n')
            i=-1;
        if (i == 1){
            i = 0;
            printf("%s", pair[0]);
            printf("%s", pair[1]);
            int idx_i = 0, idx_j = 0;
            int result = right_order(pair[0], pair[1], &idx_i, &idx_j);
            printf("%d\n", result);
            sum += result * j;
            j++;
        }
        i++;
    }

    printf("%d\n", sum);
    fclose(f);
	return 0;
}
