#include <stdio.h>

#include "io.h"
#include "chess.h"
#include "draggen.h"

/* internal variables are used to store the input */
char ch_str_move[CH_MAX_MOV_LEN+1];


/* read from the stdin row and decide whether it is a valid entry */
char *ch_get_move(char *src, char *trg)
{
	ch_error = CH_NO_ERRORS;
	*src = *trg = 0;

	/* retrieving a command to try to close the entry */
	fgets(ch_str_move, CH_MAX_MOV_LEN, stdin);
	if (feof(stdin) != 0) {
		ch_error = CH_END_OF_FILE;
		return NULL;
	}

	/* input analysis */
	switch (ch_str_move[0]) {
		/* free */
		case '\0':
			return NULL;
			break;
		/* one */
		case '\\':
			/* returns the pointer to the start of the command string */
			return (ch_str_move+1);
			break;
		default:
			/* tests the boundaries of the source field */
			if (ch_str_move[0] < 'a' || ch_str_move[0] > 'h' ||
					ch_str_move[1] < '1' || ch_str_move[1] > '8') {
				ch_error = CH_ILLEGAL_MOVE;
				return NULL;
			}
			/* calculation of field index (specified source coordinates) */
			*src = ch_str_move[0]-'a'+1 + (ch_str_move[1]-'0'+1)*10;

			/* tests the boundary of the target field */
			if (ch_str_move[2] < 'a' || ch_str_move[3] > 'h' ||
					ch_str_move[3] < '1' || ch_str_move[3] > '8') {
				ch_error = CH_ILLEGAL_MOVE;
				return NULL;
			}
			/* calculation of field index (specified target coordinates) */
			*trg = ch_str_move[2]-'a'+1 + (ch_str_move[3]-'0'+1)*10;
			break;
	}

	return NULL;
}

/* value print */
void ch_board_print_figure_place(ch_board *board)
{
	int i = 0, j = 0;

	ch_error = CH_NO_ERRORS;

	if (board == NULL) {
		ch_error = CH_NULL_POINTER;
		return;
	}

	/* print */
	printf("\nMove Number: %d\n", board->move);
	printf("    a    b    c    d    e    f    g    h\n");
	printf("-------------------------------------------\n");
	for (j = 90; j > 10; j -= 10) {
		printf("%d|", (j / 10) - 1);
		for (i = 1; i < 9; i++)
			(board->board[j + i].figure != NULL) ?
				printf("%3d |", (int)board->board[j+i].figure->type) :
				printf("    |");
		printf("%d\n", (j / 10) - 1);
		printf("-------------------------------------------\n");
	}
	printf("    a    b    c    d    e    f    g    h\n");
}

/* all possible strokes */
void ch_print_moves(ch_board *board)
{
	ch_move *move = NULL;

	if (board == NULL || board->node == NULL ||
			board->node->first == NULL || board->node->data == NULL) {
		ch_error = CH_NULL_POINTER;
		return;
	}

	board->node->actual = board->node->first;
	while (board->node->actual != NULL) {
		move = (ch_move*)(board->node->actual->data);
		printf("F%d,S%d,T%d,C%d,", board->board[(int)move->src].figure->type,move->src, 
				move->trg, move->cost);
		if (move->take != NULL) {
			printf("take%d; ", move->take->type);
		} else
			printf("take0; ");
		board->node->actual = board->node->actual->next;
	}
	putchar('\n');
	board->node->actual = board->node->first;
}

/* List the values of the playing field itself */
void ch_board_print_center_table(ch_board *board)
{
	int i = 0, j = 0;

	ch_error = CH_NO_ERRORS;
	if (board == NULL) {
		ch_error = CH_NULL_POINTER;
		return;
	}
	/* printing table */
	putchar('\n');
	for (j = 90; j > 10; j -= 10) {
		for (i = 1; i < 9; i++)
			printf("%2d", (int)board->board[j+i].value);
		putchar('\n');
	}
	putchar('\n');
}

/* write a simple table of playing fields */
/* where is the figure, then write their value */
void ch_board_print_figure_value(ch_board *board)
{
	int i = 0, j = 0;

	ch_error = CH_NO_ERRORS;
	if (board == NULL) {
		ch_error = CH_NULL_POINTER;
		return;
	}

	/* printing the table */
	putchar('\n');
	for (j = 90; j > 10; j -= 10) {
		for (i = 1; i < 9; i++)
			(board->board[j+i].figure != NULL) ?
				printf("%5d", (int)board->board[j+i].figure->value) :
				printf("     ");
		putchar('\n');
	}
	putchar('\n');
}

/* type the types of black and white figures */
/* suitable for testing correct initialization */
void ch_board_figure_print(ch_board *board)
{
	int i = 0;

	ch_error = CH_NO_ERRORS;
	if (board == NULL) {
		ch_error = CH_NULL_POINTER;
		return;
	}

	/* List of figures in the game */
	putchar('\n');
	for (i = 0; i < 16; i++) {
		printf("white: %d\n", (int)board->white[i].type);
		printf("black: %d\n", (int)board->black[i].type);
	}
	putchar('\n');
}

/* the function of error */
void ch_print_error(void)
{
	/* ending the entry is not taken as a bug, */
	/* but as a property - the possibility of ending the game with help of ^D. */
	if (ch_error == CH_END_OF_FILE) {
		ch_error = CH_NO_ERRORS;
		return;
	}
	/* own bug reports */
	printf("ERROR: ");
	switch (ch_error) {
		case CH_NOT_MEM:
			printf("can not allocate enough memmory\n");
			break;
		case CH_NULL_POINTER:
			printf("reference pointer is null\n");
			break;
		case CH_ILLEGAL_MOVE:
			printf("illegal move\n");
			break;
		default:
			printf("no errors\n");
			break;
	}
}
