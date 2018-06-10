#include <stdio.h>
#include <string.h>

#include "bitboard.h"


void printBitboard(Bitboard bitboard)
{
    int i, row = BOARD_LENGTH;
    
    for(i = BOARD_LENGTH * BOARD_LENGTH - 1; i >= 0; --i)
    {
        if((i + 1) % BOARD_LENGTH == 0)
            printf("\n%d |", row--);
        
        printf("%d|", (int)((bitboard >> i) & 0x0000000000000001));
    }
    
    printf("\n   ");
    
    char c;
    for(c = 'a'; c <= 'h'; ++c)
        printf("%c ", c);
    
    printf("\n\n");
}


void printBoardState(BoardState* boardState)
{
    char* boardName[] =
    {
        "All White Pieces",
        "All Black Pieces",
        "All Pawn Positions",
        "All Rook Positions",
        "All Knight Positions",
        "All Bishop Positions",
        "All Queen Positions",
        "All King Positions"
    };
    
    int i;
    
    printf("--- Printing out board state --- \n\n");
    
    for(i = 0; i < NUM_BITBOARDS; ++i)
    {
        printf("%s", boardName[i]);
        printBitboard(boardState->boards[i]);
    }
    
    printf("-------------------------------- \n\n");
}



Bitboard createBitboard(uint64_t squares)
{
    return squares;
}


void initBoardState(BoardState* boardState)
{
    
    boardState->flagState = 0x0000000000000000;
    
    int i,j;
    /*Initialize Castling Flag*/
    for (i = 0; i < MAX_RECURSION_DEPTH+1; i ++)
        for (j = 0; j < 4; j ++)
            boardState->castlingFlags[i][j] = 0;
    /*Initialize Enpassant Flag*/
    for (i = 0; i < MAX_RECURSION_DEPTH + 1; i++)
        boardState->enpassantFlags[i] = 0x0000000000000000;
    
    
}


void copyBoardState(BoardState* dst, BoardState* src)
{
    memcpy(dst->boards, src->boards, sizeof(Bitboard) * NUM_BITBOARDS);
}



