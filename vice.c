// vice.c

#include "stdio.h"
#include "defs.h"
#include "stdlib.h"

#define PAWNMOVESW "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define PAWNMOVESB "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"
#define KNIGHTSKINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"

int main() {	

	AllInit();		
	
	S_BOARD board[1];
	S_MOVELIST list[1];
	
	ParseFen(KNIGHTSKINGS,board);
	GenerateAllMoves(board,list);
	
	//PrintMoveList(list);
	
	return 0;
}








