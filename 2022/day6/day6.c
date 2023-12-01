#include "input.h"
#include <stdio.h>

// heavily inspired by: https://www.twitch.tv/ThePrimeagen
int
is_set_bit(char *c, int n){
    int tmp=0,prev=0;
    for (int i=0;i<n;i++){
        tmp |= 1 << (c[i]-'a');
        if (tmp==prev)
            return 0;
        prev=tmp;
    }
}

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
    for (i = 0;i<LEN-14;i++)
        if (is_set_bit(&(signal[i]),14)){
            printf("blazingly fast: %d\n",i+14);
            break;
        }
    return 0;
}
