#ifndef __CH_DRAG_GENERATOR_H__
#define __CH_DRAG_GENERATOR_H__

#include "chess.h"

typedef struct ch_move {
	char src;	/* source */
	char trg;	/* target */
	int cost;	/* stroke evaluation */
	char change;	/* change during move? */
	ch_figure *take;	/* what kind of figure during */
} ch_move;

/* possible directions and strokes of figures */
extern char ch_offset[];

/* mark threatened and not threaten in the field by the enemy */
extern void ch_gen_dangers(ch_board *board);

/* generates all possible strokes for that position */
extern void ch_gen_moves(ch_board *board);

/* generates counter-pulls for possible pulls */
extern void ch_gen_submoves(ch_board *board);

/* costing */
extern void ch_gen_cost(ch_board *board);

/* set stroke values */
extern void ch_gen_costs(ch_board *board);

/* automatic play */
extern tree *ch_gen_play(ch_board *board, char *s, char *t);

#endif
