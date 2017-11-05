#ifndef __CH_IO_H__
#define __CH_IO_H__

#include "chess.h"

#define CH_MAX_MOV_LEN 80

/* read from the stdin row and decide whether one is a valid entry */
extern char *ch_get_move(char *src, char *trg);

/* prints value */
extern void ch_board_print_figure_place(ch_board* board);

/* prints possible moves */
extern void ch_print_moves(ch_board* board);

/* error */
extern void ch_print_error(void);

/* List the values of the playing field itself */
extern void ch_board_print_center_table(ch_board *board);

/* write a simple table of playing fields */
/* where is the figure, then write their value */
extern void ch_board_print_figure_value(ch_board *board);

/* type the characters of black and white figures */
/* suitable for testing a proper initialization */
extern void ch_board_figure_print(ch_board *board);

#endif
