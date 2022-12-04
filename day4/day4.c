#include <stdio.h>

#define range_contains(x, y) (x.start<=y.start&&x.end>=y.end)
#define range_overlap(x, y) (x.start<=y.end&&x.end>=y.start)

typedef struct {
    int start;
    int end;
} range_t;

int main(){
    FILE *f = fopen("i", "r");
    range_t a, b;
    int sum = 0, overlap = 0;
    while (fscanf(f,"%d-%d,%d-%d ", &a.start, &a.end, &b.start, &b.end)==4){
        if (range_overlap(a,b))
            overlap++;
        if (range_contains(a, b) || range_contains(b, a))
            sum++;
    }
    printf("%d %d\n", sum, overlap);
}
