#include "stdio.h"
#include "defs.h"



int main() {

	AllInit();
	int index = 0;
	U64 playBitBoard = 0ULL;

	
	SETBIT(playBitBoard, 61);
	PrintBitBoard(playBitBoard);
	
	
	CLRBIT(playBitBoard, 61);
	PrintBitBoard(playBitBoard);
	
	return 0;
}
