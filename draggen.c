#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "draggen.h"
#include "ch_stddef.h"
#include "list.h"
#include "io.h"

char ch_offset[] = {
	-9, -11, 9, 11,	/* 0, 1, 2, 3, diagonal move */
	-1, 10, 1, -10,	/* 4, 5, 6, 7, horizontal and vertical move */
	19, 21, 12, -8, -19, -21, -12, 8, /* 8, 9, 10, 11, 12, 13, 14, 15 knight */
	/* part only for pawn */
	10,	/* 16 white pawn -- normal */
	20, /* 17 white pawn -- only if position equal CH_WHITE_BASE_LINE */
	11, /* 18 */
	9, /* 19 white pawn -- take alien figure */
	-10,	/* 20 black pawn -- normal */
	-20, /* 21 black pawn -- only if position equal CH_BLACK_BASE_LINE */
	-11, /* 22 */
	-9 /* 23 black pawn -- take alien figure */
};

/* mark the endangered / non-endangered field */
void ch_gen_dangers(ch_board *board)
{
	int i;
	ch_figure *figure = NULL;
	
	if (board == NULL || board->node == NULL) {
		ch_error = CH_NULL_POINTER;
		return;
	}

	/* assign the array of figures that are on the move */
	figure = (board->on_move == CH_WHITE) ? board->white : board->black;

	/* resetting the current state */
	for (i = 0; i < 120; i++)
		board->board[i].danger = 0;
	
	board->node->actual = board->node->first;
	while (board->node->actual != NULL) {
		board->board[(int)((ch_move*)(board->node->actual->data))->trg].danger = board->on_move;
		board->node->actual = board->node->actual->next;
	}
}

/* generates all possible strokes for that position */
void ch_gen_moves(ch_board *board)
{
	int start = 0, end = 0, i = 0, j = 0; 
	char typ = 0;	/* type of figure */
	ch_figure *ret = NULL;	/* pointer to figure */
	ch_figure *figure = NULL;	/* an array of figures that is on turn */
	ch_move *move = NULL;	/* create a new stroke */
	
	ch_error = CH_NO_ERRORS;

	if (board == NULL || board->node == NULL) {
		ch_error = CH_NULL_POINTER;
		return;
	}
	if (board->node->first != NULL)
		return;

	/* assign the array of figures that are on the move */
	figure = (board->on_move == CH_WHITE) ? board->white : board->black;

	/* for all figures in the field */
	for (i = 0; i < 16; i++) {
		int multi = 1; /* step counter */
		
		/* if the figure was thrown away so continue with the next one */
		if (board->board[(int)figure[i].position].value == CH_BORDER)
			continue;

		/* find type of figure (not on color) */
		typ = (figure[i].type >= 0) ? figure[i].type : (-1*figure[i].type);
		/* starting offset stroke table */
		start = (typ == CH_QUEEN || typ == CH_KING || typ == CH_BISHOP) ? 0 :
				((typ == CH_ROOK) ? 4 : ((typ == CH_KNIGHT) ? 8 : 0));
		/* end in the offset table of stroke */
		end = (typ == CH_QUEEN || typ == CH_KING || typ == CH_ROOK) ? 7 :
				((typ == CH_BISHOP) ? 3 : ((typ == CH_KNIGHT) ? 15:0));
		
		/* pawn? */
		if (typ != CH_PAWN) {
			/* for all moves to the figure */
			for (j = start; j <= end; j++) {
				int multiply = figure[i].multiply;
				multi = 1;
				ch_error = CH_NO_ERRORS;
				
				/* del */
				do {
					/* test if it's going to pull, to find the figure */
					ret = ch_figure_move(board->board, figure[i].position,
							figure[i].position + ch_offset[j]*multi, CH_TEST);
					/* if it can be dragged */
					if (ch_error == CH_NO_ERRORS) {
						/* create a new move */
						move = (ch_move*)malloc(sizeof(ch_move));
						if (move == NULL) {
							ch_error = CH_NOT_MEM;

							return;
						}
						move->src = figure[i].position;
						move->trg = figure[i].position+ch_offset[j]*multi;
						move->change = 0;
						move->take = ret;
						/* provisional value */
						move->cost = (int)(figure[i].value) +
							(int)(board->board[(int)move->trg].value);
						/* add the new stroke to the current depth of the tree */
						tree_push(board->node, (void*)move);
					} else if (ch_error != CH_ILLEGAL_MOVE) { /* if it can not be dragged */

						return;
					}
					
					/* if there is a figure in the way, 
					 * so you cannot continue */
					if (ret != NULL) {
						multiply = CH_FALSE;
					}
					/* take another step */
					multi++;
				} while (multiply == CH_TRUE && ch_error == CH_NO_ERRORS);
			} /* the end of each turn of the figure */
		} else {	/* if pawn */
			char tmp_pos = figure[i].position; /* position change */
			int fw = (figure[i].type>0) ? 16:20,	/* pull: forward */
				fw2 = (figure[i].type>0) ? 17:21,	/* 	2x forward */
				fwt1 = (figure[i].type>0) ? 18:22,	/* 	go right */
				fwt2 = (figure[i].type>0) ? 19:23,	/*	go left */
				basln = (figure[i].type>0) ?
					tmp_pos - CH_WHITE_BASE_LINE :
					tmp_pos - CH_BLACK_BASE_LINE;		/* ditermine */

			/* Test if you can pull the given direction and find the figure gates */
			ret = ch_figure_move(board->board, tmp_pos, tmp_pos+ch_offset[fw], CH_TEST);
			/* if you can go there */
			if (ch_error == CH_NO_ERRORS && ret == NULL) {
				/* create a new move */
				move = (ch_move*)malloc(sizeof(ch_move));
				if (move == NULL) {
					ch_error=CH_NOT_MEM;
					printf("\n\nIn ch_gen_moves();\nTyp: %d\nmove == NULL\n\n", typ);
					return;
				}
				move->src = tmp_pos;
				move->trg = tmp_pos+ch_offset[fw]*multi;
				move->change = 0;
				move->take = ret;
				move->cost = (int)(figure[i].value) +
					(int)(board->board[(int)move->trg].value);
				/* move new to the current depth of the tree */
				tree_push(board->node, (void*)move);

			} else if (ch_error!=CH_NO_ERRORS && ch_error!=CH_ILLEGAL_MOVE) {
				/* mistake? */
				printf("\n\nIn ch_gen_moves();\nLine: 100\nTyp: %d\nch_error == %d\n\n", typ, ch_error);
				return;
			}
			
			/* check done */
			if (basln<10 && basln>0 && board->board[tmp_pos+ch_offset[fw]].figure==NULL) {
				/* is it possible to pull two steps? */
				ret = ch_figure_move(board->board, tmp_pos, tmp_pos+ch_offset[fw2], CH_TEST);
				/* nothing can stand in the way */
				if (ch_error == CH_NO_ERRORS && ret == NULL) {
					/* new move */
					move = (ch_move*)malloc(sizeof(ch_move));
					if (move == NULL) {
						ch_error = CH_NOT_MEM;
						printf("\n\nIn ch_gen_moves();\nTyp: %d\nch_error == %d\nin 0<basln<10\n\n", typ, ch_error);
						return;
					}
					move->src = tmp_pos;
					move->trg = tmp_pos+ch_offset[fw2]*multi;
					move->change = 0;
					move->take = ret;
					move->cost = (int)(figure[i].value) +
						(int)(board->board[(int)move->trg].value);
					/* Add to tree */
					tree_push(board->node, (void*)move);

				} else if (ch_error != CH_NO_ERRORS && ch_error != CH_ILLEGAL_MOVE) {
					/* mistake? */
					printf("\n\nIn ch_gen_moves();\nTyp: %d\nch_error == %d\nin 0<basln<10 -- second\n\n", typ, ch_error);
					return;
				}
			}
			
			/* try */
			for (j = fwt1; j <= fwt2; j++) {
				ret = ch_figure_move(board->board, tmp_pos, tmp_pos+ch_offset[j], CH_TEST);
				/* is there is a figure? */
				if (ret != NULL) {
					/* new move */
					move = (ch_move*)malloc(sizeof(ch_move));
					if (move == NULL) {
						ch_error = CH_NOT_MEM;
						printf("\n\nIn ch_gen_moves();\nTyp: %d\nch_error == %d\nin 0<basln<10 -- second\n\n", typ, ch_error);
						return;
					}
					move->src = tmp_pos;
					move->trg = tmp_pos+ch_offset[j];
					move->change = 0;
					move->take = ret;
					move->cost = (int)(figure[i].value) +
						(int)(board->board[(int)move->trg].value);
					/* place new move */
					tree_push(board->node, (void*)move);

				}
			}
		}
		/* for all error figures */
		/* the error status can be reset when you move to the next figure */ 
		ch_error = CH_NO_ERRORS;
	}


}

/* generates counter-pulls for possible pulls */
void ch_gen_submoves(ch_board *board)
{
	int best = 0;	/* best move */
	ch_move *move = NULL;	/* for pull */
	
	/* will not want to work with indifferent markers??? */
	if (board == NULL || board->node == NULL || board->node->first == NULL) {
		ch_error=CH_NULL_POINTER;
		printf("ch_gen_submoves: null pointer %s\n", (board) ? ((board->node) ? "board->node->first" : "board->node") : "board");
		return;
	}

	/* value individual moves */
	ch_gen_costs(board); 

	/* temporarily change */
	board->on_move = (board->on_move == CH_WHITE) ? CH_BLACK : CH_WHITE;

	/* take first move */
	board->node->actual = board->node->first;

	/* deduce all possible moves */
	while (board->node->actual != NULL) {
		/* make move in tree */
		board->node = board->node->actual;
		/*** check up to this poing ***/
		move = (ch_move*)board->node->data;
		/* make a move, do not have to figure out what it is
		 * already discovered by the generator */
		ch_figure_move(board->board, move->src, move->trg, CH_DO_IT);
		/* generate all possible counter-forces to make the move */
		ch_gen_moves(board);
		/* adjust the stroke values */
		ch_gen_costs(board);

		if (board->node->first == NULL) {
			/* win condition? */
			break;
		}

		/* finds the lowest value move
		 * assume enemy pulls*/
		board->node->actual = board->node->first;
		best = ((ch_move*)board->node->actual->data)->cost;
		while (board->node->actual != NULL) {
			if (best < ((ch_move*)board->node->actual->data)->cost)
				best = ((ch_move*)board->node->actual->data)->cost;
			board->node->actual = board->node->actual->next;
		}
		
		/* undo */
		/* ch_board_undo(board); */
		ch_figure_move(board->board, move->trg, move->src, CH_DO_IT);
		/* if anything is taken, the figure needs to be reinstated */
		if (move->take != NULL) {
			move->take->position = move->trg;
			board->board[(int)move->take->position].figure = move->take;
		}
		/* it turns back to the original tree */
		board->node = board->node->root;
		
		/* we find the value found as the cost of the stroke */
		((ch_move*)board->node->actual->data)->cost += -1*best;
		
		/* another move */
		board->node->actual = board->node->actual->next;
	}

	/* back to orighinal */
	board->on_move = (board->on_move == CH_WHITE) ? CH_BLACK : CH_WHITE;

	/* because the board-> node-> current is now NULL, so set it to the first move */
	board->node->actual = board->node->first;
	/* ch_board_print_figure_place(board); */
}

/* balance */
void ch_gen_cost(ch_board *board)
{
	int i = 0;
	
	if (board == NULL) {
		ch_error=CH_NULL_POINTER;
		return;
	}
	
	/* reset status values */
	board->wcost = 0;
	board->bcost = 0;
	
	/* gradually read the current state into the variables */
	for (i = 0; i < 16; i++) {
		if (board->white[i].position != 0)
			board->wcost += board->white[i].value +
				CH_WHITE * board->board[(int)board->white[i].position].value;
		if (board->black[i].position!=0)
			board->bcost += board->black[i].value +
				CH_BLACK * board->board[(int)board->black[i].position].value;
	}

	/* difference values (number is invalid?) */
	board->cost = board->wcost + board->bcost;


}

/* set stroke values */
void ch_gen_costs(ch_board *board)
{
	tree *node = NULL;	/* pointer to tree */
	ch_move* move = NULL;	/* pointer to move */
	int color = 0;		/* player color */
	ch_field *b = NULL;	/* pointer on playing feild */

	if (board == NULL || board->node == NULL || board->node->first == NULL ||
			board->node->first->data == NULL) {
		ch_error=CH_NULL_POINTER;
		printf("ch_gen_costs: %s\n", (board) ? ((board->node) ? "board->node->first" : "board->node") : "board");
		return;
	}

	node = board->node->first;
	b = board->board;

	color = board->on_move;

	/* till stroke pass */
	while (node != NULL) {
		move = (ch_move*)(node->data);
		/* target field value - source field value = gain (or loss) */
		move->cost = b[(int)(move->trg)].value - b[(int)(move->src)].value;
		/* worth it's own value? */
		if (b[(int)(move->trg)].figure != NULL) {
			/* the value is positive and black will be inhibited */
			/* different color than an opponent, so I neglect my color */
			move->cost += (-1*color) * b[(int)(move->trg)].figure->value;
		}

		/* next? (to be checked) */
		node = node->next;
	}
}

/* automatic play */
tree *ch_gen_play(ch_board *board, char *s, char *t)
{
	tree *node = NULL; /* pointer to the tree */
	tree *best = NULL; /* node with best play */
	
	if (board == NULL || board->node == NULL || board->node->first == NULL) {
		ch_error = CH_NULL_POINTER;
		return best;
	}

	node = board->node;
	node->actual = node->first;	/* start with first play */
	best = node->actual;
	
	/* go through move list */
	while (node->actual != NULL) {
		/* better move? (incorrect logic to be rechecked) */
		if (((ch_move*)(best->data))->cost < ((ch_move*)(node->actual->data))->cost) {
			best = node->actual;
		} else if (((ch_move*)(best->data))->cost == ((ch_move*)(node->actual->data))->cost &&
				random()%2 == 0) {
			best = node->actual;
		}
		node->actual = node->actual->next;
	}
	/* let the pointer point to NULL */
	board->node->actual = node->first;
	/* empty? */
	if (best == NULL) {
		*s = 0;
		*t = 0;
	} else {
		/* set selected cordinates */
		*s = ((ch_move*)(best->data))->src;
		*t = ((ch_move*)(best->data))->trg;
	}

	return best;
}
