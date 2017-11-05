#ifndef __CH_CHESS_H__
#define __CH_CHESS_H__

#include "tree.h"

/* macro for initialization */
#define ch_figure_init(fig, typ, val, pos, mov, multi) \
	(fig).type = (char)(typ); \
	(fig).value = (short int)(val); \
	(fig).position = (char)(pos); \
	(fig).move = (char)(mov); \
	(fig).multiply = (char)(multi)

/* macro for positioning a figure on a playing field 
 * when successful, returns the link to the figure touched
 * otherwise, if there is something back, there is a link to the figure that stands there
 * otherwise return NULL */
#define ch_figure_set(b, f) ((b)[(int)(f)->position].figure != NULL || \
				(b)[(int)(f)->position].value == CH_BORDER || \
				(f)->position < 0 || (f)->position > 119) ? \
				(b)[(int)(f)->position].figure : \
			        ((b)[(int)(f)->position].figure = f)

/* macro for error printing */
#define ch_error_print() if (ch_error!=CH_NO_ERRORS) ch_print_error()
				
typedef int player;

/* structure describing the figure */
typedef struct ch_figure {
	char type;	/* kind of figure */
	short int value;	/* value */
	char position;	/* current position */
	unsigned char move;
	char multiply;	/* can you do the next step */
} ch_figure;

/* one playing field */
typedef struct ch_field {
	ch_figure *figure;	/* what is there behind the figure */
	char value;	/* feild value */
	int danger;	/* weather the feild is under threat figure */
} ch_field;

/* structure describing the game */
typedef struct ch_board {
	ch_figure white[16];	/* white figures */
	ch_figure black[16];	/* black figures */
	ch_field board[120];	/* chessboard */
	char on_move;			/* which color to play */
	int move;				/* move */
	int check;				/* chaeck */
	player wp;				/* white play */
	player bp;				/* black play */
	player pl;				/* which player to move */
	int wcost;				/* white costing */
	int bcost;				/* black costing */
	int cost;				/* costing */
	tree *node;				/* tree for move */
} ch_board;


/* error constans */
typedef enum {
	CH_NO_ERRORS, CH_NOT_MEM, CH_NULL_POINTER, CH_ILLEGAL_MOVE, CH_END_OF_FILE
} ch_eerror;

/* error change */
extern ch_eerror ch_error;

/* initialization of the ch_board structure */
extern ch_board *ch_board_init(ch_board *board);

/* end of game - tree is over */
extern void ch_board_done(ch_board *board);

/* the beginning of the game */
extern void ch_game_start(ch_board *board);

/* move the figure */
extern inline ch_figure* ch_figure_move(ch_field board[], char src, char target, int flag);

/* undo */
extern void ch_board_undo(ch_board *board);
	
#endif
