#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "../aoc.h"

#ifdef DEBUG
	#define FILENAME "test"
	#define X 13
	#define Q 169
#else
	#define FILENAME "i"
	#define X 141
	#define Q 19881
#endif

typedef
struct {
	Point p;
	int dist;
	Direction d;
	int steps;
} Node;

Point moves[4] = {
	{ 0, -1 }, // UP
	{ 0, 1 }, // DOWN
	{ -1, 0 }, // LEFT
	{ 1, 0 }, // RIGHT
};

static void inline
print_map( int map[X][X]) {
	for ( int i = 0; i<X; ++i ) {
	for ( int j = 0; j<X; putchar(map[i][j++]+'0'));
	putchar('\n');
	}
}

static void inline
print_vis( char map[X][X] ) {
	for ( int i = 0; i<X; ++i ) {
	for ( int j = 0; j<X; putchar(map[i][j++]));
	putchar('\n');
	}
}

Node
heap_pop_min( Node *heap, int *h_size ) {
	int i, index, d_min = INT_MAX;
	for ( i = 0; i<*h_size; ++i) {
		if ( heap[i].dist < d_min ) {
			d_min = heap[i].dist;
			index = i;
		}
	}
	Node n = heap[index];
	for ( i = index; i < *h_size-1; ++i )
		heap[i] = heap[i+1];
	(*h_size)--;
	return n;
}

void
print_heap ( Node *heap, int h_size ) {
	for ( int i = 0; i<h_size; ++i ) {
		printf("{x = %d; y = %d; dist = %d, direction = %d} ", heap[i].p.x, heap[i].p.y, heap[i].dist, heap[i].d);
	} 
	putchar('\n');
}

void
get_directions( int directions[3], int steps, Direction d ) {
	for ( int i = 0; i<3; ++i ) directions[i] = -1;
	switch (d) {
	case N:
	case S:
		directions[0] = 2;
		directions[1] = 3;
		break;
	case E:
	case W:
		directions[0] = 0;
		directions[1] = 1;
		break;
	}
	if ( steps == 3 ) return;
	directions[2] = d;
}

void
print_path( Point parents[X][X], char vis[X][X], int x, int y ) {
	vis[y][x] = '.';
	if (parents[y][x].y == -1 && parents[y][x].x == -1) return;
	print_path(parents, vis, parents[y][x].x, parents[y][x].y);
}

int
main(int argc, char *argv[]){

	FILE *fp = fopen(FILENAME, "r");

	int map[X][X];
	char vis[X][X];
	char c;

	int i = 0, j = 0;
	while ( (c = fgetc(fp)) != EOF ) {
		if ( c == '\n' ) {
			i++;
			j = 0;
		}
		else {
			map[i][j] = c-'0';
			vis[i][j] = c;
			j++;
		}
	}

	// I think we need to use Dijkstra
	int dists[X][X];
	Point parents[X][X];
	bool visited[X][X];

	int steps = 0;
	Direction d = E;
	int directions[3];

	Node *heap = malloc( Q * sizeof(Node) );
	int h_size = 0;

	for (int i = 0; i<X; ++i) {
		for ( int j = 0; j<X; ++j ) {
			dists[i][j] = INT_MAX;
			visited[i][j] = false;
		}
	}

	dists[0][0] = 0;
	parents[0][0].x = -1;
	parents[0][0].y = -1;

	dists[0][1] = 0;
	parents[0][1].x = 0;
	parents[0][1].y = 0;

	dists[1][0] = 0;
	parents[1][0].x = 0;
	parents[1][0].y = 0;

	Node n;
	// n.p.x = 0; n.p.y = 0; n.d = E; n.dist = 0; n.steps = 0;
	n.p.x = 1; n.p.y = 0; n.d = E; n.dist = 0; n.steps = 0;
	heap[h_size++] = n;

	n.p.x = 0; n.p.y = 1; n.d = S; n.dist = 0; n.steps = 0;
	heap[h_size++] = n;
	
	while ( h_size > 0 ) {

		n = heap_pop_min(heap, &h_size);

		if ( n.p.x == X-1 && n.p.y == X-1 ) continue;

		get_directions(directions, n.steps, n.d);	

		int nx, ny;

		for ( int i = 0; directions[i] != -1 && i<3; ++i ) {

			nx = n.p.x + moves[directions[i]].x;
			ny = n.p.y + moves[directions[i]].y;


			if ( nx < 0 || nx == X || ny < 0 || ny == X ) continue;

			if ( dists[ny][nx] > dists[n.p.y][n.p.x] + map[ny][nx] ) {

				dists[ny][nx] = dists[n.p.y][n.p.x] + map[ny][nx];

				Node nn;
				nn.p.x = nx; nn.p.y = ny; nn.dist = dists[ny][nx]; nn.d = directions[i];

				if ( n.d == nn.d ) nn.steps = n.steps+1;
				else nn.steps = 0;

				heap[h_size++] = nn;

				parents[nn.p.y][nn.p.x].x = n.p.x; 
				parents[nn.p.y][nn.p.x].y = n.p.y; 

		/*		for ( int i = 0; i<X; ++i ) 
					for ( int j = 0; j<X; ++j )
						vis[i][j] = map[i][j];
				print_path(parents, vis, X-1, X-1);
				print_vis(vis);
		*/

			} 
		}
	}

	free(heap);

	print_path(parents, vis, X-1, X-1);
	print_vis(vis);

	for ( int i = 0; i<X; ++i ) {
		for ( int j = 0; j<X; printf("%3d ", dists[i][j++]));
		putchar('\n');
	}
	
	fclose(fp);

	return 0;
}
