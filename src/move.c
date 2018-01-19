

#include <stdlib.h>

#include "move.h"


void initMoves(Moves* moves)
{
  moves->quietMoves = moves->captureMoves = NULL;
}

void cleanUpMoves(Moves* moves)
{
  // FILL THIS IN LATER
}


void initPossibleMoves(PossibleMoves* possibleMoves)
{
  int i;
  for(i = 0; i < NUM_PIECES; ++i)
    possibleMoves->possibleMoves[i] = NULL;
}


void cleanUpPossibleMoves(PossibleMoves* possibleMoves)
{
  int i;
  for(i = 0; i < NUM_PIECES; ++i)
    free(possibleMoves->possibleMoves[i]);
}





PossibleMoves* generatePossibleMoves(BoardState* boardState, enum PlayerType playerType)
{
  PossibleMoves* posibleMoves = (PossibleMoves*) malloc(sizeof(PossibleMoves));
  posibleMoves->possibleMoves[PIECE_TYPE_PAWN]   = generatePawnMoves(boardState, playerType);
  posibleMoves->possibleMoves[PIECE_TYPE_ROOK]   = generateRookMoves(boardState, playerType);
  posibleMoves->possibleMoves[PIECE_TYPE_KNIGHT] = generateKnightMoves(boardState, playerType);
  posibleMoves->possibleMoves[PIECE_TYPE_BISHOP] = generateBishopMoves(boardState, playerType);
  posibleMoves->possibleMoves[PIECE_TYPE_QUEEN]  = generateQueenMoves(boardState, playerType);
  posibleMoves->possibleMoves[PIECE_TYPE_KING]   = generateKingMoves(boardState, playerType);
  return posibleMoves;
}









Moves* generatePawnMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}


Moves* generateRookMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}


Moves* generateKnightMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}


Moves* generateBishopMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}


Moves* generateQueenMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}


Moves* generateKingMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}














Moves* generateSlideMoves(BoardState* boardState, enum PlayerType playerType)
{
  
  
  return NULL;
}


Moves* generateDiagonalMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}


Moves* generateLMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}


Moves* generateStepMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}
