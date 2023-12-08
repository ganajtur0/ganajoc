#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
	#define NODES 8
	#define FILENAME "test"
#else
	#define NODES 814
	#define FILENAME "i"
#endif 

static inline int
namecmp(char name1[3], char name2[3]) {
	return (name1[0] == name2[0] &&
	        name1[1] == name2[1] &&
		name1[2] == name2[2]);
}

typedef
struct node {
	char name[3];
	char l_name[3];
	char r_name[3];
} Node;

Node
parse_node( char *line ) {
	Node node;
	char *iter = line;
	for (int i = 0; i<3; node.name[i++] = *(iter++));
	iter+=4;	
	for (int i = 0; i<3; node.l_name[i++] = *(iter++));
	iter+=2;
	for (int i = 0; i<3; node.r_name[i++] = *(iter++));
	return node;
}

void
print_node( Node node ) {
	for (int i = 0; i<3; putc(node.name[i++], stdout));
	printf(" = (");
	for (int i = 0; i<3; putc(node.l_name[i++], stdout));
	putc(',', stdout);
	for (int i = 0; i<3; putc(node.r_name[i++], stdout));
	printf(")\n");
}

int
node_index( Node nodes[NODES], char name[3] ) {
	for (int i = 0; i<NODES; ++i) {
		if (namecmp(nodes[i].name, name))
			return i;
	}
	return -1;
}

void
part_1_solution( char *directions, Node nodes[NODES] ) {

	char *iter = directions;

	char AAA[3] = { 'A', 'A', 'A' };
	char ZZZ[3] = { 'Z', 'Z', 'Z' };

	int aaa = node_index(nodes, AAA);
	int zzz = node_index(nodes, ZZZ);

	int steps = 0;

	int current_node = aaa;
	while (current_node != zzz) {
	// 	print_node(nodes[current_node]);
		switch (*iter) {
			case 'R':
				current_node = node_index(nodes, nodes[current_node].r_name);
				break;
			case 'L':
				current_node = node_index(nodes, nodes[current_node].l_name);
				break;
			case '\n':
				iter = directions;
				continue;
			default:
				break;
		}
		iter++;
		steps++;
	}
	printf("Part 1 solution: %d\n", steps);
}

void
part_2_solution( char *directions, Node nodes[NODES] ) {


	char *iter = directions;
	int steps = 0;

	int n_s[NODES];
	int n_s_len = 0;
	int z_s[NODES];
	int z_s_len = 0;
	
	for ( int i = 0; i<NODES; ++i) {
		if (nodes[i].name[2] == 'A') n_s[n_s_len++] = i;
		else if (nodes[i].name[2] == 'Z') z_s[z_s_len++] = i;
	}

	int done = 0, escape = 0;
	while ( !done ) {

		for ( int i = 0; i<n_s_len; ++i) {
			switch (*iter) {
				case 'R':
					n_s[i] = node_index(nodes, nodes[n_s[i]].r_name);
					if (n_s[i] == -1) return;
					break;
				case 'L':
					n_s[i] = node_index(nodes, nodes[n_s[i]].l_name);
					if (n_s[i] == -1) return;
					break;
				case '\n':
					iter = directions;
					escape = 1;
					break;
				default:
					break;
			}
		}
		if (escape) { escape = 0; continue; }
		
		printf("\n%c\n", *iter);

		iter++;
		steps++;

		for (int i = 0; i<n_s_len; i++) print_node(nodes[n_s[i]]);


		for ( int i = 0; i<n_s_len; ++i) {
			int found = 0;
			for ( int j = 0; j<z_s_len; ++j) {
				if (n_s[i] == z_s[i]) {
					found = 1;
					break;
				}
			}
			done *= found;
		}
	}

	printf("Part 2 solution: %d\n", steps);
}

int
main(int argc, char *argv[]){

	FILE *fp;

	char *instructions = NULL;
	size_t instructions_len;

	char *line = NULL;
	size_t line_len;
	ssize_t read;

	Node nodes[NODES];
	int nodes_index = 0;

	fp = fopen(FILENAME, "r");

	getline( &instructions, &instructions_len,fp );

	getline( &line, &line_len, fp );
	
	while ( (read = getline(&line, &line_len, fp)) != -1 ) {
		nodes[nodes_index++] = parse_node(line);
	}

	// for (int i = 0; i<NODES; print_node(nodes[i++]));

	printf("directions: %s\n", instructions);

	// part_1_solution(instructions, nodes);
	part_2_solution(instructions, nodes);

	free(instructions);
	free(line);

	fclose(fp);

	return 0;
}
