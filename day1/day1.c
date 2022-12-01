// cSaK ESTÉDIJÓ!!
#include <stdio.h>

int tenpow(int p){
    int r = 1;
    for (r = 1; p>0; r*=10, p--);
    return r;
}

void
strip(char *str){
    char *iter = str;
    for (;*iter != '\n'; iter++)
        if (*iter == '\0') return;
    *iter = '\0';
}

char
str_len(char *str){
    int len = 0;
    for (char *iter = str;
         *iter != '\0';
         iter++, len++);
    return len;
}

int
parse_cal(char *cal_str){
    int parsed = 0;
    char len = str_len(cal_str)-1;
    for (char *iter = cal_str;
         *iter != '\0';
         iter++, len--)
        parsed += ((*iter)-'0')*tenpow(len);
    return parsed;
}

int main(){

    FILE *f = fopen("i", "r");
    char line[10];
   
    int calories = 0,
        first = 0,
        second = 0,
        third = 0;

    while (fgets(line, 10, f) != (void*)(0)){
        if (line[0] == '\n') {

            if (calories > first){
                second = first;
                first = calories;
            }
            else if (calories > second){
                third = second;
                second = calories;
            }
            else if (calories > third)
                third = calories;

            calories = 0;
            continue;
        }
        strip(line);
        calories += parse_cal(line);
    }
    printf("%d %d\n", first, first+second+third);

    return 0;
}
