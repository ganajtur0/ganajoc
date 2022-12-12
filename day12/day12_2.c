#include <stdio.h>

#ifdef TEST
    #define W 8
    #define H 5
#else
    #define W 144
    #define H 41
#endif

#define SIZE (W*H)

typedef
struct _q_item {
    int dist;
    int row;
    int col;
} q_item;

typedef
struct {
	q_item items[5904];
	int count;
} q;

q_item
q_item_create(int row, int col, int dist){
    q_item r;
    r.row = row;
    r.col = col;
    r.dist = dist;
    return r;
}

void
q_append(q *q, q_item v){
	q->items[(q->count)++]=v;
}

q_item *
q_front(q *q){
	q_item *tmp = &(q->items[0]);
	for (int i = 1; i<q->count; i++)
		q->items[i-1] = q->items[i];
	(q->count)--;
	return tmp;
}

void
q_print(q q){
	for (int i = 0; i<q.count; i++)
		printf("%p ", q.items[i].dist);
	printf("\n");
}

void
map_print(char map[H][W]){
	for (int i = 0; i<H;i++){
		for (int j = 0; j<W;j++)
			putc(map[i][j], stdout);
		putc('\n', stdout);
	}
}

void
map_visited_print(int map[H][W]){
	for (int i = 0; i<H;i++){
		for (int j = 0; j<W;j++)
			printf("%d", map[i][j]);
		putc('\n', stdout);
	}
}

int main(){
#ifdef TEST
	FILE *f = fopen("test", "r");
	char buf[10];
#else
	FILE *f = fopen("i", "r");
	char buf[256];
#endif

	char map[H][W];
    int map_visited[H][W] = {0};
	int h = 0;

    q q;
    q.count = 0;

#ifdef TEST
	while ((fgets(buf, 10, f)!=(void*)(0))){
#else
	while ((fgets(buf, 256, f)!=(void*)(0))){
#endif
		for (int i = 0; buf[i]!='\n'; i++){
			map[h][i]=buf[i];
            if (buf[i]=='S')
                map[h][i] = 'a';
            else if (buf[i]=='E'){
                map[h][i] = '{';
                map_visited[h][i] = 1;
                q_append(&q, q_item_create(h, i, 0));
            }
        }
		h++;
	}

    int distance = -1;
    q_item *p;
    while (q.count > 0){

        p = q_front(&q);

        if (map[p->row][p->col] == 'a'){
            distance = p->dist;
            break;
        }

        // ^
        if ( p->row-1 >= 0 && (map[p->row-1][p->col]-map[p->row][p->col])<=-1
             && map_visited[p->row-1][p->col] == 0){
            q_append(&q, q_item_create(p->row-1, p->col, p->dist+1));
            map_visited[p->row-1][p->col] = 1;
        }

        // ˇ
        if ( p->row+1 < H && (map[p->row+1][p->col]-map[p->row][p->col])<=-1
             && map_visited[p->row+1][p->col] == 0){
            q_append(&q, q_item_create(p->row+1, p->col, p->dist+1));
            map_visited[p->row+1][p->col] = 1;
        }

        // <
        if ( p->col-1 >= 0 && (map[p->row][p->col-1]-map[p->row][p->col])<=-1
             && map_visited[p->row][p->col-1] == 0){
            q_append(&q, q_item_create(p->row, p->col-1, p->dist+1));
            map_visited[p->row][p->col-1] = 1;
        }

        // <
        if ( p->col+1 < W && (map[p->row][p->col+1]-map[p->row][p->col])<=-1
             && map_visited[p->row][p->col+1] == 0){
            q_append(&q, q_item_create(p->row, p->col+1, p->dist+1));
            map_visited[p->row][p->col+1] = 1;
        }
        map_visited_print(map_visited);
        // putc('\n', stdout);

    }

    printf("total steps taken: %d\n", distance);
	return 0;
}
