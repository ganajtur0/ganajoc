// csak estédijó
#include <stdio.h>

void
strip(char *str){
    char *iter = str;
    for (;*iter != '\n'; iter++)
        if (*iter == '\0') return;
    *iter = '\0';
}

int
str_len(char *str){
    int len = 0;
    for (char *iter = str;
         *iter != '\0';
         iter++, len++);
    return len;
}

int
str_contains(char *s, char c){
    for (char *iter = s;
         *iter != '\0';
         iter++) if (*iter == c) return 1;
    return 0;
}

int
priority(char c){
    if (!(c & 0x20))
        return c-'&';
    return c-'`';
}

int
rucksack(char *s){
    int len = str_len(s);
    int idx = len/2, i = 0;
    for (;i<idx; i++)
        if (str_contains(&(s[idx]), s[i]))
            return priority(s[i]);
    return 0;
}

int
group(char *o, char *tw, char *th){
    for (char *i=o;*i!='\0';i++){
        if(str_contains(tw, *i) && str_contains(th, *i))
            return priority(*i);
    }
    return 0;
}

int main(){
    FILE *f = fopen("i", "r");
    char i[100];
    int sum = 0;
    while (fgets(i, 100, f) != (void*)(0)){
        strip(i);
        sum+=rucksack(i);
    }
    printf("%d ", sum);
    rewind(f);
    sum = 0;
    char o[100], tw[100], th[100];
    while (fgets(o, 100, f) != (void*)(0) &&
           fgets(tw, 100, f) != (void*)(0) &&
           fgets(th, 100, f) != (void*)(0)){
        strip(o); strip(tw); strip(th);
        sum += group(o, tw, th);
    }
    printf("%d\n", sum);
    fclose(f);
}
