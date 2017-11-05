#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "io.h"
#include "chess.h"
#include "ch_stddef.h"
#include "draggen.h"
#include "tree.h"

/* error change */
ch_eerror ch_error = CH_NO_ERRORS;

/* initialization of the ch_board structure */
ch_board *ch_board_init(ch_board *board)
{
	int i = 0, j = 0;
	
	ch_error = CH_NO_ERRORS;

	/* if the structure is not created, one creates */
	if (board == NULL)
		if ((board = malloc(sizeof(ch_board))) == 0) {
			ch_error = CH_NOT_MEM;
			return NULL;
		}

	/* we initialize all borders as if they were walls */
	for (i = 0; i < 120; i++) {
		board->board[i].figure = NULL;
		board->board[i].value = CH_BORDER;
		board->board[i].danger = 0;
	}

	/* Create a free play area */
	for (j = 20; j < 100; j += 10)
		for (i = 1; i < 9; i++)
			board->board[j + i].value = CH_VAKAT;

	/* Create a tree */
	board->node = tree_new(malloc(sizeof(ch_move)));
	if (board->node == NULL)
		return board;
	if (board->node->data == NULL){
		free(board->node);
		return board;
	}
	((ch_move*)board->node->data)->src = 0;
	((ch_move*)board->node->data)->trg = 0;
	((ch_move*)board->node->data)->cost = 0;
	((ch_move*)board->node->data)->change = 0;

	/* initialization of figures */
	for (i = 0; i < 8; i++) {
		ch_figure_init(board->white[i], CH_PAWN, PAWN, CH_A2+i, CH_PM, CH_FALSE);
		ch_figure_init(board->black[i], CH_BLACK_PAWN, BLACK_PAWN, CH_A7+i, CH_PM, CH_FALSE);
	}
	ch_figure_init(board->white[8], CH_ROOK, ROOK, CH_A1, CH_VH, CH_TRUE);
	ch_figure_init(board->white[9], CH_ROOK, ROOK, CH_H1, CH_VH, CH_TRUE);
	ch_figure_init(board->black[8], CH_BLACK_ROOK, BLACK_ROOK, CH_A8, CH_VH, CH_TRUE);
	ch_figure_init(board->black[9], CH_BLACK_ROOK, BLACK_ROOK, CH_H8, CH_VH, CH_TRUE);
	ch_figure_init(board->white[10], CH_KNIGHT, KNIGHT, CH_B1, CH_KM, CH_FALSE);
	ch_figure_init(board->white[11], CH_KNIGHT, KNIGHT, CH_G1, CH_KM, CH_FALSE);
	ch_figure_init(board->black[10], CH_BLACK_KNIGHT, BLACK_KNIGHT, CH_B8, CH_KM, CH_FALSE);
	ch_figure_init(board->black[11], CH_BLACK_KNIGHT, BLACK_KNIGHT, CH_G8, CH_KM, CH_FALSE);
	ch_figure_init(board->white[12], CH_BISHOP, BISHOP, CH_C1, CH_DIAG, CH_TRUE);
	ch_figure_init(board->white[13], CH_BISHOP, BISHOP, CH_F1, CH_DIAG, CH_TRUE);
	ch_figure_init(board->black[12], CH_BLACK_BISHOP, BLACK_BISHOP, CH_C8, CH_DIAG, CH_TRUE);
	ch_figure_init(board->black[13], CH_BLACK_BISHOP, BLACK_BISHOP, CH_F8, CH_DIAG, CH_TRUE);
	ch_figure_init(board->white[14], CH_QUEEN, QUEEN, CH_D1, CH_DIAG|CH_VH, CH_TRUE);
	ch_figure_init(board->white[15], CH_KING, KING, CH_E1, CH_DIAG|CH_VH, CH_FALSE);
	ch_figure_init(board->black[14], CH_BLACK_QUEEN, BLACK_QUEEN, CH_D8, CH_DIAG|CH_VH, CH_TRUE);
	ch_figure_init(board->black[15], CH_BLACK_KING, BLACK_KING, CH_E8, CH_DIAG|CH_VH, CH_FALSE);

	/* umist figures */
	for (i = 0; i < 16; i++) {
		ch_figure_set(board->board, &board->white[i]);
		ch_figure_set(board->board, &board->black[i]);
	}

	/* set the values of the individual cells for the center game */
	i=CH_A1;
	board->board[i++].value = 4;
	board->board[i++].value = 0;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	board->board[i++].value = 0;
	board->board[i++].value = 4;
	i=CH_A2;
	board->board[i++].value = 4;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	board->board[i++].value = 4;
	i=CH_A3;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 20;
	board->board[i++].value = 20;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	i=CH_A4;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 20;
	board->board[i++].value = 24;
	board->board[i++].value = 24;
	board->board[i++].value = 20;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	i=CH_A5;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 20;
	board->board[i++].value = 24;
	board->board[i++].value = 24;
	board->board[i++].value = 20;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	i=CH_A6;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 20;
	board->board[i++].value = 20;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	i=CH_A7;
	board->board[i++].value = 4;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 16;
	board->board[i++].value = 16;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	board->board[i++].value = 4;
	i=CH_A8;
	board->board[i++].value = 4;
	board->board[i++].value = 0;
	board->board[i++].value = 8;
	board->board[i++].value = 12;
	board->board[i++].value = 12;
	board->board[i++].value = 8;
	board->board[i++].value = 0;
	board->board[i++].value = 4;

	/* nobody has one at the beginning */
	board->check = 0;

	/* the first one will be white */
	board->on_move = CH_WHITE;

	board->wp = CH_HUMAN;	/* a man will play for the game */
	board->bp = CH_CPU;		/* for a black computer */
	board->pl = board->wp;	/* the stroke is full */

	board->move = 1;		/* First move */

	/*ch_gen_cost(board);*/

	return board;
}

/* end of the game - the tree is over */
void ch_board_done(ch_board *board)
{
	if (board == NULL)
		return;
	/* when the tree exists */
	if (board->node != NULL) {
		/* Scearch roots */
		while (board->node->root != NULL)
			board->node = board->node->root;
		/* Deleat all branches */
		tree_del(board->node);
		board->node=NULL;
	}
}

/* The beginning of the game */
void ch_game_start(ch_board *board)
{
	char src = 0, trg = 0; /* where to where */
	char *str = NULL;	/* command */
	int end = 0;	/* a condition that determines whether or not the prize is yet to be played */
	int found = CH_FALSE;	/* if the move was found in the list of possible strokes */
	
	if (board == NULL || board->node == NULL) {
		ch_error = CH_NULL_POINTER;
		return;
	}
	
	/* Check */
	ch_board_print_figure_place(board);

	/* until the end */
	while (!end) {
		found = CH_FALSE;
		
		/* the user in the previous iteration of the cycle could have entered an invalid input / move */
		if (ch_error == CH_NO_ERRORS) {
			/* generate all possible moves for that position */
			ch_gen_moves(board);
			/* find out how we are on it */
			ch_gen_cost(board);
			/* generate additional levels */
			ch_gen_submoves(board);
			/*ch_gen_costs(board); */
			ch_gen_dangers(board);
		}
		
		/* the move will be cleared from the entrance */
		if (board->pl == CH_HUMAN) {
			str = ch_get_move(&src, &trg);
		} else {
			/* the move will be counted. from where to save src, trg
			 * and as a return value is a reference to the drag in the tree */
			board->node->actual = ch_gen_play(board, &src, &trg);
			/* could not generate something rational to generate */
			if (board->node->actual != NULL) {
				/* we generate only valid moves,
				 * therefore there is no need to verify the validity of the move */
				found = CH_TRUE;
			} else {
				/* when we could not generate anything, we lost */
				end = 1;
				continue;
			}
		}
		
		/* end of entry or play how to play (lost) */
		if (ch_error == CH_END_OF_FILE || board->node->first == NULL) {
			end = 1;
			continue;
		} else if (ch_error != CH_NO_ERRORS) {
			/* there was some error, so we'll write it out */
			ch_error_print();
			continue;
		}
		
		/* not move but command? */
		if (str != NULL) {
			if (strcmp(str, "quit\n") == 0) {
				end = 1;
				ch_error = CH_NO_ERRORS;
			} else if (strcmp(str, "moves\n") == 0) {
				ch_print_moves(board);
			} else if (strcmp(str, "undo\n") == 0) {
				ch_board_undo(board);
				ch_board_undo(board);
				ch_board_print_figure_place(board);
			}
			/* remove the generated strokes and do not generate again */
			tree_del_nodes(board->node);
			continue;
		}
		
		/* if the stroke moves automatically */
		if (found != CH_TRUE)
			/* Set first stroke */
			board->node->actual = board->node->first;
		/* we will look for a list of required moves */
		while (found == CH_FALSE && board->node->actual != NULL) {
			/* this is somewhat strange, so it's rather surprising */
			if (board->node->actual->data == NULL) {
				board->node->actual = board->node->actual->next;
				continue;
			}
			/* Self test */
			if ((((ch_move*)(board->node->actual->data))->src == src) &&
				(((ch_move*)(board->node->actual->data))->trg == trg))
				found = CH_TRUE;
			else	/* if it is not out, then another move */
				board->node->actual = board->node->actual->next;
		}
		
		/* if the move was not on the list */
		if (found==CH_FALSE) {
			board->node->actual = board->node->first;
			ch_error = CH_ILLEGAL_MOVE;
		} else {/* the move was on the list, so I set the position in the tree */
			tree *pom = NULL;
			/* to find a pull from a tree */
			pom = tree_rem(board->node->actual);
			/* remove other moves from the tree - unnecessarily blocking the memory */
			tree_del_nodes(board->node);
			/* Re-insert the saved move */
			tree_add_first(board->node, pom);
			/* has moved the current floor to move */
			board->node = pom;
			/* delete all branches of the move - we will generate them again */
			/*tree_del_nodes(board->node);*/
			found = CH_FALSE;
		}
		
		/* if doted there was a mistake and was not resolved, so her listing */
		if (ch_error != CH_NO_ERRORS) {
			if (str == NULL && ch_error == CH_END_OF_FILE) {
				end = 1;
			}
			ch_error_print();
		} else {
			/* when there is no back error, one is a valid move and we do it*/
			ch_figure_move(board->board, src, trg, CH_DO_IT);
			/* change the color that is on the move */
			board->on_move = (board->on_move == CH_WHITE) ? CH_BLACK : CH_WHITE;
			/* change the game that is on the move */
			board->pl = (board->on_move == CH_WHITE) ? board->wp : board->bp;
			/* if there was any error while moving, then the listing to what */
			if (ch_error != CH_NO_ERRORS) {
				ch_error_print();
			} else {
				/* the stroke was successful, so it is in/possible to draw a new shuffle */
				ch_board_print_figure_place(board);
				/* when an error occurs, write it for what else otherwise */
				ch_error_print();
			}
		}
		
		/* we will go to the next play */
		board->move++;
	}
}

/* move the figure */
inline ch_figure* ch_figure_move(ch_field board[120], char src, char target, int flag)
{
	ch_figure *tmp = NULL;
	ch_figure *figure = NULL;
	ch_figure* ret = NULL;	/* return value - Branch Figure */
	char s;	/* preserved source position */

	ch_error = CH_NO_ERRORS;
	if (board == NULL || board[(int)src].figure == NULL) {
		ch_error = CH_NULL_POINTER;
		return ret;
	}
	
	/* taking note behind figure */
	figure = board[(int)src].figure;
	/* Save original position */
	s = figure->position;
	/* set positon to move */
	figure->position = target;

	/* test on the endangered field */
	if (figure->type == CH_KING || figure->type == CH_BLACK_KING) {
		if (board[(int)target].danger == 0 ||
			(board[(int)target].danger<0 && figure->type<0) ||
			(board[(int)target].danger>0 && figure->type>0)) {
			tmp = ch_figure_set(board, figure);
		} else {
			ch_error = CH_ILLEGAL_MOVE;
			figure->position = s;
			return ret;
		}
	} else {
		tmp = ch_figure_set(board, figure);
	}
	
	/* if a move has been made outside the playing board */
	if (tmp == NULL) {
		ch_error = CH_ILLEGAL_MOVE;
		/* restore position */
		figure->position = s;
		return ret;
	}
	/* when there is something else and it's not me - so I did not move */
	if (tmp != figure) {
		/* is this my figure? */
		if ((tmp->type>0 && figure->type>0) || (tmp->type<0 && figure->type<0)) {
			/* It was my figure */
			ch_error = CH_ILLEGAL_MOVE;
			figure->position = s;
			return ret;
		} else {
			ret = tmp;
			if (flag & CH_DO_IT) { /* not my figure but pick up? */
				tmp->position = 0;	/* the enemy figure */
			}	/* otherwise one is just a test */
		}
	}
	
	/* should you really pull? */
	if (flag & CH_DO_IT) {
		/* hence, completed the transfer maneuver */
		board[(int)target].figure = figure;	/* the field on the board contains a figure */
		board[(int)s].figure = NULL;	/* the original field has been emptied */
	} else {	/* we're just testing */
		figure->position = s;	/* restore position */
		if (tmp == figure) {	/* when the shipment took place */
			board[(int)target].figure = NULL;	/* we clean the target field */
		}
	}

	/* the gate of the figure */
	return ret;
}

/* undo */
void ch_board_undo(ch_board *board)
{
	ch_move* move; /* return the move */
	
	if (board == NULL || board->node == NULL || board->node->root == NULL) {
		ch_error = CH_NULL_POINTER;
		return;
	}

	move = (ch_move*)board->node->data;
	/* perform a return movement - you have to tell the story, because the pult before it happened there */
	ch_figure_move(board->board, move->trg, move->src, CH_DO_IT);
	/* if anything was taking place */
	if (move->take != NULL) {
		printf("ahoj");
		/* return the figures */
		move->take->position = move->trg;
		board->board[(int)move->take->position].figure = move->take;
	}

	/* returns back to the tree of the level */
	board->node = board->node->root;
	/* Unwanted strokes */
	tree_del_nodes(board->node);
}
