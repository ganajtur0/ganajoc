#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define IS_DIGIT(X) ( X >= '0' && X <= '9' )

typedef enum {
	NONE,
	MUL,
	L_SIDE,
	COMMA,
	R_SIDE,
	CLOSE,
	DOIN
} state_e;

typedef struct {
	state_e state;
	char buf_l[4];
	char buf_r[4];
	int buf_l_i;
	int buf_r_i;
	char prev;
	char doinit[8];
	int doindex;
	int enabled;
} ctx_t;

static inline void
ctx_reset(ctx_t *ctx) {
	ctx->state = NONE;
	memset(ctx->buf_l, 0, 4);
	memset(ctx->buf_r, 0, 4);
	memset(ctx->doinit, 0, 8);
	ctx->buf_l_i = 0;
	ctx->buf_r_i = 0;
	ctx->prev = 0;
	ctx->doindex = 0;
}

int
parse(ctx_t *ctx, char c) {

	if (IS_DIGIT(c)) {
		switch (ctx->state) {
			case NONE:
				return -1;
			case MUL:
				return -1;
			case L_SIDE:
			case COMMA:
				ctx->buf_l[ctx->buf_l_i++] = c;
				ctx->state = COMMA;
				return 0;
			case R_SIDE:
			case CLOSE:
				ctx->buf_r[ctx->buf_r_i++] = c;
				ctx->state = CLOSE;
				return 0;
			default:
				return 0;
		}
	}
	else {
		switch (c) {
		case 'm':
			if (ctx->prev != 0)
				return -1;
			ctx->state = MUL;
			ctx->prev  = 'm';
			return 0;
		case 'u':
			if (ctx->prev != 'm')
				return -1;
			ctx->prev = 'u';
			return 0;
		case 'l':
			if (ctx->prev != 'u')
				return -1;
			ctx->prev = 'l';
			return 0;
		case '(':
			if (ctx->state == DOIN)
				goto doitboy;
			if (ctx->prev != 'l')
				return -1;
			ctx->state = L_SIDE;
			return 0;
		case ',':
			if (ctx->state != COMMA)
				return -1;
			ctx->state = R_SIDE;
			return 0;
		case ')':
			if (ctx->state == DOIN)
				goto doitboy;
			if (ctx->state != CLOSE)
				return -1;
			return ( atoi(ctx->buf_l) * atoi(ctx->buf_r) );
		case 'd':
			if (ctx->state != NONE)
				return -1;
			ctx->state = DOIN;
			ctx->doinit[ctx->doindex++] = 'd';
			return 0;
		default:
			if (ctx->state != DOIN)
				return -1;
		doitboy:
			ctx->doinit[ctx->doindex++] = c;
			if (strncmp(ctx->doinit, "don't()", ctx->doindex) != 0) {
				if (strncmp(ctx->doinit, "do()", ctx->doindex) != 0) {
					return -1;
				}
				else if (ctx->doindex == 4) {
					ctx->enabled = 1;
					return -1;
				}
				else {
					return 0;
				}
			}
			else if (ctx->doindex == 7) {
				ctx->enabled = 0;
				return -1;
			}
			else {
				return 0;
			}
		}
	}
}

int
main(int argc, char *argv[]){

	FILE *fp;

	#ifdef DEBUG
	fp = fopen("test", "r");
	#else
	fp = fopen("i", "r");
	#endif

	ctx_t ctx;
	ctx_reset(&ctx);
	ctx.enabled = 1;

	char c;

	int result, sum = 0;

	while ((c = fgetc(fp)) != EOF){
		result = parse(&ctx, c);
		if (result == -1)
			ctx_reset(&ctx);
		else if (result != 0) {
			sum += result * ctx.enabled;
			ctx_reset(&ctx);
		}
	}

	// printf("PART 1: %d\n", sum);
	printf("PART 2: %d\n", sum);

	return 0;
}
