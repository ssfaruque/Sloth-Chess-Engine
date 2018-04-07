
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * Description of Chess Engine HERE                                             *
 ********************************************************************************/


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
/*
  boardState->boards[BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS] = 0x000000000000ffff;
  boardState->boards[BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS] = 0xffff000000000000;
  boardState->boards[BOARD_TYPE_ALL_PAWN_POSITIONS]         = 0x00ff00000000ff00;
  boardState->boards[BOARD_TYPE_ALL_ROOK_POSITIONS]         = 0x8100000000000081;
  boardState->boards[BOARD_TYPE_ALL_KNIGHT_POSITIONS]       = 0x4200000000000042;
  boardState->boards[BOARD_TYPE_ALL_BISHOP_POSITIONS]       = 0x2400000000000024;
  boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS]        = 0x1000000000000010;
  boardState->boards[BOARD_TYPE_ALL_KING_POSITIONS]         = 0x0800000000000008;
  */
  boardState->flagState                                     = 0x0000000000000000;

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



