

#include <stdio.h>

#include "bitboard.h"


void printBitboard(Bitboard* bitboard)
{
  int i, row = BOARD_LENGTH;
  
  for(i = BOARD_LENGTH * BOARD_LENGTH - 1; i >= 0; --i)
  {
    if((i + 1) % BOARD_LENGTH == 0)
      printf("\n%d |", row--);

    printf("%d|", (int)((bitboard->squares >> i) & 0x0000000000000001));
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
  
  for(i = 0; i < NUM_BITBOARDS; ++i)
  {
    printf("%s", boardName[i]);
    printBitboard(&boardState->boards[i]);
  }
}



void initBitboard(Bitboard* bitboard, uint64_t squares)
{
  bitboard->squares = squares;
}


void initBoardState(BoardState* boardState)
{
  initBitboard(&boardState->boards[BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS], 0x000000000000ffff);
  initBitboard(&boardState->boards[BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS], 0xffff000000000000);
  initBitboard(&boardState->boards[BOARD_TYPE_ALL_PAWN_POSITIONS],         0x00ff00000000ff00);
  initBitboard(&boardState->boards[BOARD_TYPE_ALL_ROOK_POSITIONS],         0x8100000000000081);
  initBitboard(&boardState->boards[BOARD_TYPE_ALL_KNIGHT_POSITIONS],       0x4200000000000042);
  initBitboard(&boardState->boards[BOARD_TYPE_ALL_BISHOP_POSITIONS],       0x2400000000000024);
  initBitboard(&boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS],        0x1000000000000010);
  initBitboard(&boardState->boards[BOARD_TYPE_ALL_KING_POSITIONS],         0x0800000000000008);
}


void clearBoardState(BoardState* boardState)
{
  int i;
  for(i = 0; i < NUM_BITBOARDS; ++i)
    boardState->boards[i].squares = 0;
}


void printBitboardRow(Bitboard* bitboard, int row)
{
  int i;
  printf("Printing row %d of bitboard\n", row);
  
  for(i = BOARD_LENGTH - 1; i >=0; --i)
  {
    printf("%d", (bitboard->rows[row - 1] >> i) & 1);
  }
  
  printf("\n");
}


