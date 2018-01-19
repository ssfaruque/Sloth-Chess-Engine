

#ifndef move_h
#define move_h

#include "bitboard.h"

enum PlayerType
{
  PLAYER_TYPE_WHITE,
  PLAYER_TYPE_BLACK
};

enum PieceType
{
  PIECE_TYPE_PAWN,
  PIECE_TYPE_ROOK,
  PIECE_TYPE_KNIGHT,
  PIECE_TYPE_BISHOP,
  PIECE_TYPE_QUEEN,
  PIECE_TYPE_KING,
  NUM_PIECES
};


typedef struct
{
  BoardState* quietMoves;
  BoardState* captureMoves;
} Moves;


typedef struct
{
  Moves* possibleMoves[NUM_PIECES];
} PossibleMoves;



void initMoves(Moves* moves);
void cleanUpMoves(Moves* moves);
void initPossibleMoves(PossibleMoves* possibleMoves);
void cleanUpPossibleMoves(PossibleMoves* possibleMoves);


PossibleMoves* generatePossibleMoves(BoardState* boardState, enum PlayerType playerType);

Moves* generatePawnMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateRookMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateKnightMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateBishopMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateQueenMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateKingMoves(BoardState* boardState, enum PlayerType playerType);

Moves* generateSlideMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateDiagonalMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateLMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateStepMoves(BoardState* boardState, enum PlayerType playerType);


#endif /* move_h */
