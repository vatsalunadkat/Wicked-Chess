// vice.c

#include "stdio.h"
#include "defs.h"
#include "stdlib.h"

#define FEN1 "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 2 "
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

void ShowSqAtBySide(const int side, const S_BOARD *pos) {
		
	int rank = 0;
	int file = 0;
	int sq = 0;
	
	printf("\n\nSquares attacked by:%c\n",SideChar[side]);
	for(rank = RANK_8; rank >= RANK_1; --rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = FR2SQ(file,rank);
			if(SqAttacked(sq,side,pos)==TRUE) {
				printf("X");
			} else {
				printf("-");
			}
				
		}
		printf("\n");
	}  
    printf("\n\n");

}


int main() {	

	AllInit();	

	S_BOARD board[1];
	
	ParseFen(FEN1, board);	
	PrintBoard(board);	
	//ASSERT(CheckBoard(board));
	
	printf("\n\nWhite Attacking:\n");
	ShowSqAtBySide(WHITE,board);
	
	printf("\n\nBlack Attacking:\n");
	ShowSqAtBySide(BLACK,board);
	return 0;
}








