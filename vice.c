// vice.c

#include "stdio.h"
#include "defs.h"
#include "stdlib.h"

#define CASTLE1 "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
#define CASTLE2 "3rk2r/8/8/8/8/8/6p1/R3K2R b KQk - 0 1"

int main() {	

	AllInit();		
	
	S_BOARD board[1];
	S_MOVELIST list[1];
	
	ParseFen(CASTLE2,board);
	GenerateAllMoves(board,list);
	
	//PrintMoveList(list);
	
	return 0;
}








