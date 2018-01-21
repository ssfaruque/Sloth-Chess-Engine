

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


typedef uint64_t Bitboard;


typedef struct
{
  Bitboard boards[NUM_BITBOARDS];
} BoardState;


Bitboard createBitboard(uint64_t squares);
void initBoardState(BoardState* boardState);
void copyBoardState(BoardState* dst, BoardState* src);
void printBitboard(Bitboard bitboard);
void printBoardState(BoardState* boardState);



#endif /* bitboard_h */


