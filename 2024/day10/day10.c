#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef DEBUG
#define SIZE 8
#else
#define SIZE 55
#endif

#define BFS_MAX SIZE*SIZE

#define ABS(X) ( (X) < 0 ? (X) * -1 : (X) )

#define COLOR_BOLD "\e[1m"
#define COLOR_RED  "\e[31m"
#define COLOR_OFF  "\e[m"

void
visualize(int map[SIZE][SIZE], bool visited[SIZE][SIZE]) {
	for ( int i = 0; i<SIZE; i++ ) {
		for ( int j = 0; j<SIZE; j++ ) {
			if (visited[i][j]) {
				printf(COLOR_BOLD COLOR_RED "%d" COLOR_OFF, map[i][j]);
			}
			else
				printf("%d", map[i][j]);
		}
		putchar('\n');
	}
}

int
bfs_p1(int map[SIZE][SIZE], int s_y, int s_x) {

	int count = 0;

	int q[BFS_MAX][2], front = 0, rear = 0;

	bool visited[SIZE][SIZE] = {false};

	visited[s_y][s_x] = true;

	q[rear][0]   = s_y;
	q[rear++][1] = s_x;

	while ( front < rear ) {

		// printf("Visiting y : %d and x : %d\n", q[front][0], q[front][1]);

		int curr_y = q[front][0];
		int curr_x = q[front++][1];

		for ( int i = -1; i<=1; i++) {
			for ( int j = -1; j<=1; j++ ) {
				if ( i != 0 && j != 0 ) continue;
				int n_y = curr_y + i;
				int n_x = curr_x + j;
				if ( n_y < 0 || n_y >= SIZE || n_x < 0 || n_x >= SIZE
				     || visited[n_y][n_x] || (map[n_y][n_x] - map[curr_y][curr_x]) !=  1 )
					continue;
				if (map[n_y][n_x] == 9 && !visited[n_y][n_x]){
					visited[n_y][n_x] = true;
					count++;
					continue;
				}
				q[rear][0] = n_y;
				q[rear++][1] = n_x;
				visited[n_y][n_x] = true;
				// if (map[n_y][n_x] == 9) count++;
			}
		} 
	}

	// visualize(map, visited);

	return count;
}

int
bfs_p2(int map[SIZE][SIZE], int s_y, int s_x) {

	int count = 0;

	int q[BFS_MAX][2], front = 0, rear = 0;

	// bool visited[SIZE][SIZE] = {false};

	// visited[s_y][s_x] = true;

	q[rear][0]   = s_y;
	q[rear++][1] = s_x;

	while ( front < rear ) {

		// printf("Visiting y : %d and x : %d\n", q[front][0], q[front][1]);

		int curr_y = q[front][0];
		int curr_x = q[front++][1];

		for ( int i = -1; i<=1; i++) {
			for ( int j = -1; j<=1; j++ ) {
				if ( i != 0 && j != 0 ) continue;
				int n_y = curr_y + i;
				int n_x = curr_x + j;
				if ( n_y < 0 || n_y >= SIZE || n_x < 0 || n_x >= SIZE
				     || /*visited[n_y][n_x] ||*/ (map[n_y][n_x] - map[curr_y][curr_x]) !=  1 )
					continue;
				if (map[n_y][n_x] == 9){
					// visited[n_y][n_x] = true;
					count++;
					continue;
				}
				q[rear][0] = n_y;
				q[rear++][1] = n_x;
				// visited[n_y][n_x] = true;
				// if (map[n_y][n_x] == 9) count++;
			}
		} 
	}

	// visualize(map, visited);

	return count;
}

void
print_map(int map[SIZE][SIZE]) {
	for ( int i = 0; i<SIZE; i++ ) {
		for ( int j = 0; j<SIZE; j++ ) {
			printf("%d", map[i][j]);
		}
		putchar('\n');
	}
}

int
part1(int map[SIZE][SIZE]) {
	int result = 0;
	for ( int i = 0; i<SIZE; i++ ) {
		for ( int j = 0; j<SIZE; j++ ) {
			if (map[i][j] == 0)
				result += bfs_p1(map, i, j);
		}
	}
	return result;
}

int
part2(int map[SIZE][SIZE]) {
	int result = 0;
	for ( int i = 0; i<SIZE; i++ ) {
		for ( int j = 0; j<SIZE; j++ ) {
			if (map[i][j] == 0)
				result += bfs_p2(map, i, j);
		}
	}
	return result;
}

int
main(int argc, char *argv[]){

	FILE *fp;

	#ifdef DEBUG
	fp = fopen("test", "r");
	#else
	fp = fopen("i", "r");
	#endif

	if (fp == NULL) return 1;

	int map[SIZE][SIZE];
	char x;

	int r = 0, c = 0;

	while ( (x = fgetc(fp)) != EOF ) {
		if ( x == '\n' ) {
			r++;
			c=0;
		}
		else {
			map[r][c++] = x - '0';
		}
	}

	printf("PART 1 : %d\n", part1(map));
	printf("PART 2 : %d\n", part2(map));

	return 0;
}
