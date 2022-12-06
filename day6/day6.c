#include "input.h"
#include <stdio.h>

int
is_set(char *c, int n){
    for (int i=0;i<n;i++) for (int j=i+1;j<n;j++)
        if (c[i]==c[j])
            return 0;
    return 1;
}

int main(){
    int i;
    for (i = 0;i<LEN-4;i++)
        if (is_set(&(signal[i]),4)){
            printf("%d ",i+4);
            break;
        }
    for (i = 0;i<LEN-14;i++)
        if (is_set(&(signal[i]),14)){
            printf("%d\n",i+14);
            break;
        }
    return 0;
}
