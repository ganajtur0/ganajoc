#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef DEBUG
#define SIZE 10
#else
#define SIZE 140
#endif

#define BFS_MAX SIZE*SIZE

#define IN_BOUNDS(X, Y) ( (X) >= 0 && (X) < SIZE && (Y) >= 0 && (Y) < SIZE )

void
print_map(char map[SIZE][SIZE]) {
	for ( int i = 0; i<SIZE; i++ ) {
		for ( int j = 0; j<SIZE; j++ ) {
			putchar(map[i][j]);
		}
		putchar('\n');
	}
}

int
bfs_p1(char map[SIZE][SIZE], bool visited[SIZE][SIZE], int s_y, int s_x, char c) {
	
	int count = 1;
	int perim = 0;

	int q[BFS_MAX][2], front = 0, rear = 0;

	// bool visited[SIZE][SIZE] = {false};

	visited[s_y][s_x] = true;

	q[rear][0]   = s_y;
	q[rear++][1] = s_x;

	while ( front < rear ) {

		// printf("Visiting y : %d and x : %d\n", q[front][0], q[front][1]);

		int curr_y = q[front][0];
		int curr_x = q[front++][1];

		int neighbors = -1;

		for ( int i = -1; i<=1; i++) {
			for ( int j = -1; j<=1; j++ ) {
				if ( i != 0 && j != 0 ) continue;
				int n_y = curr_y + i;
				int n_x = curr_x + j;

				if ( n_y >= 0 && n_y < SIZE && n_x >= 0 && n_x < SIZE
					&& map[n_y][n_x] ==  c )
					neighbors++;

				if ( n_y < 0 || n_y >= SIZE || n_x < 0 || n_x >= SIZE ||
				visited[n_y][n_x] || map[n_y][n_x] !=  c )
					continue;

				count++;
				q[rear][0] = n_y;
				q[rear++][1] = n_x;
				visited[n_y][n_x] = true;
				// if (map[n_y][n_x] == 9) count++;
			}
		}


		perim+=(4-neighbors);
	}

	// printf("A region of [%c] plants with price\t%d * %d = %d\n", c, count, perim, count * perim);
	// visualize(map, visited);

	return count * perim;
}

int
corners(bool map[SIZE][SIZE], int y, int x) {

	int n_corners = 0;

	//.............................TOP......RIGHT...DOWN.....LEFT...
	int neighbor_offset[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };
	bool ns[4];
	int nc = 0;

	for ( int i = y; i<SIZE; i++ ) {
		for ( int j = x; j<SIZE; j++ ) {
			if (!map[i][j]) continue;
			for ( int n = 0; n<4; n++ ) {
				int n_y = i + neighbor_offset[n][0];
				int n_x = j + neighbor_offset[n][1];

				ns[n] = (IN_BOUNDS(n_y, n_x) && map[n_y][n_x]);
				nc += (int)ns[n];
			}
			printf("%d %d neighbors:\t", i, j);
			for ( int bruh = 0; bruh < 4; bruh++ ) {
				printf(ns[bruh] ? "true " : "false ");
			}
			putchar('\n');
			if ( nc == 2 && ( (!ns[0] && !ns[1]) || (!ns[1] && !ns[2]) || (!ns[2] && !ns[3]) || (!ns[3] && !ns[1]) ) ) n_corners++;
			else if (nc == 1) n_corners+=2;
			else if (nc == 0) n_corners+=4;
		}
	}
	return n_corners;
}

int
bfs_p2(char map[SIZE][SIZE], bool visited[SIZE][SIZE], int s_y, int s_x, char c) {
	
	bool area[SIZE][SIZE] = {false};

	int count = 1;
	int perim = 0;

	int q[BFS_MAX][2], front = 0, rear = 0;

	// bool visited[SIZE][SIZE] = {false};

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

				if ( n_y < 0 || n_y >= SIZE || n_x < 0 || n_x >= SIZE ||
				visited[n_y][n_x] || map[n_y][n_x] !=  c )
					continue;

				count++;
				q[rear][0] = n_y;
				q[rear++][1] = n_x;
				visited[n_y][n_x] = true;
				area[n_y][n_x] = true;
				// if (map[n_y][n_x] == 9) count++;
			}
		}


		// perim+=(4-neighbors);
	}
	perim+=corners(area, s_y, s_x);

	printf("A region of [%c] plants with price\t%d * %d = %d\n", c, count, perim, count * perim);
	// visualize(map, visited);

	return count * perim;
}

int
part1(char map[SIZE][SIZE]) {
	int count = 0;
	bool visited[SIZE][SIZE] = {false};
	for ( int i = 0; i < SIZE; i++ ) {
		for ( int j = 0; j<SIZE; j++ ) {
			if (visited[i][j]) continue;
			count += bfs_p1(map, visited, i, j, map[i][j]);
		}
	}
	return count;
}

int
part2(char map[SIZE][SIZE]) {
	int count = 0;
	bool visited[SIZE][SIZE] = {false};
	for ( int i = 0; i < SIZE; i++ ) {
		for ( int j = 0; j<SIZE; j++ ) {
			if (visited[i][j]) continue;
			count += bfs_p2(map, visited, i, j, map[i][j]);
		}
	}
	return count;
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

	char map[SIZE][SIZE];

	char x;
	int r = 0, c = 0;

	while ( (x = fgetc(fp)) != EOF ) {
		if ( x == '\n' ) {
			r++; c = 0;
		}
		else {
			map[r][c++] = x;
		}
	}

	fclose(fp);

	printf("PART 1 : %d\n", part1(map));
	printf("PART 2 : %d\n", part2(map));

	return 0;
}
