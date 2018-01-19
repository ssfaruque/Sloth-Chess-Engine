

#ifndef bitboard_h
#define bitboard_h

#include <stdint.h>


#define BOARD_LENGTH 8

enum BitboardType
{
  BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS,
  BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS,
  BOARD_TYPE_ALL_PAWN_POSITIONS,
  BOARD_TYPE_ALL_ROOK_POSITIONS,
  BOARD_TYPE_ALL_KNIGHT_POSITIONS,
  BOARD_TYPE_ALL_BISHOP_POSITIONS,
  BOARD_TYPE_ALL_QUEEN_POSITIONS,
  BOARD_TYPE_ALL_KING_POSITIONS,

  NUM_BITBOARDS
};


typedef union
{
  uint64_t squares;
  uint8_t rows[BOARD_LENGTH];
} Bitboard;


typedef struct
{
  Bitboard boards[NUM_BITBOARDS];
} BoardState;


void initBitboard(Bitboard* bitboard, uint64_t squares);
void initBoardState(BoardState* boardState);
void clearBoardState(BoardState* boardState);
void printBitboard(Bitboard* bitboard);
void printBoardState(BoardState* boardState);
void printBitboardRow(Bitboard* bitboard, int row);


#endif /* bitboard_h */
