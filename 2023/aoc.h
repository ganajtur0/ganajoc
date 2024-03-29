#ifndef AOC_H
#define AOC_H

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
size_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}
#endif

#define MAX(x, y) ( x > y ? x : y )
#define MIN(x, y) ( x < y ? x : y )

#define PRINT_ERROR(...) fprintf(stderr, __VA_ARGS__)

typedef
enum {
    N,
    S,
    W,
    E
} Direction;

typedef
struct {
    int x;
    int y;
} Point;

typedef
struct {
    int width;
    int height;
    int line_num;
    char **matrix;
} CharMatrix;

// if the height is zero, the default (10) height will be used
CharMatrix
charmatrix_create( const int width, const int height ) {
    CharMatrix cm;
    cm.width = width;
    cm.height = (height ? height : 10);
    cm.line_num = 0;
    cm.matrix = (char**)malloc( cm.height * sizeof(char*) );
    for ( int i = 0; i<cm.height; ++i )
        cm.matrix[i] = (char*)malloc( cm.width );
    return cm;
}

int
charmatrix_add_line ( CharMatrix *cm, char *const line, const char line_end ) {
    if ( (cm->line_num) == (cm->height) ) {
        (cm->height)+=10;
        char **newptr = (char**)realloc( cm->matrix, cm->height * sizeof(char*) );
        if ( newptr == NULL ) {
            PRINT_ERROR("Allocation for CharMatrix failed!\n");
            free(cm->matrix);
            return -1;
        }
        cm->matrix = newptr;
        for ( int i = cm->line_num; i<cm->height; ++i ) {
            cm->matrix[i] = (char*)malloc( cm->width );
        }
    }
    char *iter = line;
    for ( int i = 0; i < cm->width && *iter != line_end; ++i, ++iter ) {
        ((cm->matrix)[(cm->line_num)])[i] = *iter;
    }
    (cm->line_num)++;
    return cm->height;
}

void
charmatrix_free( CharMatrix *cm ) {
    for ( int i = 0; i<cm->line_num; ++i ) {
        free(cm->matrix[i]);
    }
    free(cm->matrix);
}

void
charmatrix_print( CharMatrix cm ) {
    if (cm.line_num == -1) printf("-- Empty --");
    else {
        for ( int i = 0; i<cm.line_num; ++i ) {
            for ( int j = 0; j<cm.width; putchar(cm.matrix[i][j++]));
            putchar('\n');
        }
    }
}

static inline void
char2d_print( char **m, int y, int x ) {
    for ( int i = 0; i<y; ++i ) {
        for ( int j = 0; j<x; putchar(m[i][j++]));
        putchar('\n');
    }
}

static inline int
line_is_empty( char *line ) {
	while ( *line != '\0' ){
		line++;
		if (!isspace(*line))
			return 0;
	}
	return 1;
}

unsigned long long
eat_the_rich(char **line) {
	char buffer[100] = {0};
	while (!isdigit(**line)) {
		(*line)++;
		if (**line == '\n' || **line == '\0')
			return -1;
	}
	for (int i = 0; **line != '\0'; (*line)++) {
		if (isdigit(**line))
			buffer[i++] = **line;
	}
	return strtoull(buffer, NULL, 10);
}

unsigned long
eat_next_unsigned_long(char **line) {

	char buffer[20] = {0};
	while (!isdigit(**line)) {
		(*line)++;
		if (**line == '\n' || **line == '\0')
			return -1;
	}
	for (int i = 0; !(isspace(**line)); buffer[i++] = **line, (*line)++);
	return strtoul(buffer, NULL, 10);
}

int
eat_next_int(char **line, int *could_parse) {

	char buffer[20] = {0};
	for (; !isdigit(**line) && **line != '-'; (*line)++) {
		if (**line == '\n' || **line == '\0') {
			*could_parse = 0;
			return -1;
		}
	}
	for (int i = 0; isdigit(**line); buffer[i++] = **line, (*line)++);
	*could_parse = 1;
	return atoi(buffer);
	
}

#endif //AOC_H
