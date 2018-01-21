

#include <stdio.h>
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


void updateBoardState(BoardState* boardState, Bitboard initialPiece, Bitboard movedPiece,
                      enum BitboardType colorType, enum BitboardType pieceType)
{
  // only quiet moves for now
  boardState->boards[colorType] = ((boardState->boards[colorType] ^ initialPiece) | movedPiece);
  boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ initialPiece) | movedPiece);
  
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


/******************************** Generate moves by pieces ********************************/


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


/******************************************************************************************/



/******************************** Moves pertaining to slides ********************************/


Moves* generateSlideMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}



Moves* generateSlideLeftMoves(BoardState* boardState, enum BitboardType colorType,
                              enum BitboardType pieceType)
{
  Bitboard isolatedPieces = boardState->boards[colorType] & boardState->boards[pieceType];
  
  //while(isolatedPieces)
  //{
  //  int offset = 1;
  
    Bitboard isolatedPiece = isolatedPieces & -isolatedPieces;
  
    Move* move = generateSlideLeftMove(isolatedPiece, boardState, colorType, pieceType, 1);
  
  
  
  
  
    printBoardState(move);
    
    
    
    //while(move)
    //{
      
    //}
    
    
    
    // reset ls1b
  //  isolatedPieces &= isolatedPieces - 1;
  //}
  
  return NULL;
}



Move* generateSlideLeftMove(Bitboard isolatedPiece, BoardState* boardState, enum BitboardType colorType,
                             enum BitboardType pieceType, int offset)
{
  Move* move = (Move*) malloc(sizeof(Move));
  copyBoardState(move, boardState);
  
  Bitboard movedPiece = isolatedPiece << offset;
  Bitboard collisions = movedPiece & boardState->boards[colorType];
  
  // check for collision with own piece and out of bounds conditions
  if(!collisions && !(movedPiece & 0x0101010101010101))
  {
    return move;
  }
  
  free(move);
  return NULL;
}



Moves* generateSlideRightMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}



Moves* generateSlideRightMove(BoardState* boardState, enum PlayerType playerType, int offset)
{
  return NULL;
}



Moves* generateSlideUpMoves(BoardState* boardState, enum BitboardType colorType,
                            enum BitboardType pieceType)
{
  Bitboard isolatedPieces = boardState->boards[colorType] & boardState->boards[pieceType];
  
  //while(isolatedPieces)
  //{
  //  int offset = 1;
  
  Bitboard isolatedPiece = isolatedPieces & -isolatedPieces;
  
  Bitboard movedPiece = generateSlideUpMove(isolatedPiece, boardState, colorType, pieceType, 7);

  
  printf("Initial board state\n");
  printBoardState(boardState);
  
  
  if(!movedPiece)
    updateBoardState(boardState, isolatedPiece, movedPiece, colorType, pieceType);
  
  
  
  
  printf("Piece moved up\n");
  printBoardState(boardState);
  
  
  
  //while(move)
  //{
  
  //}
  
  
  
  // reset ls1b
  //  isolatedPieces &= isolatedPieces - 1;
  //}
  
  return NULL;
}



Bitboard generateSlideUpMove(Bitboard isolatedPiece, BoardState* boardState, enum BitboardType colorType,
                           enum BitboardType pieceType, int offset)
{
  Bitboard movedPiece = isolatedPiece << (8 * offset);
  Bitboard collisions = movedPiece & boardState->boards[colorType];
  
  // check for collision with own piece and out of bounds conditions
  if(!collisions && movedPiece)
  {
    return movedPiece;
  }
  
  return 0;
}



Moves* generateSlideDownMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}



Moves* generateSlideDownMove(BoardState* boardState, enum PlayerType playerType, int offset)
{
  return NULL;
}


/******************************************************************************************/




/******************************** Moves pertaining to diagonals ********************************/



Moves* generateDiagonalMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}



Moves* generateDiagonalUpRightMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}



Moves* generateDiagonalUpRightMove(BoardState* boardState, enum PlayerType playerType, int offset)
{
  return NULL;
}



Moves* generateDiagonalUpLeftMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}



Moves* generateDiagonalUpLeftMove(BoardState* boardState, enum PlayerType playerType, int offset)
{
  return NULL;
}



Moves* generateDiagonalDownRightMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}



Moves* generateDiagonalDownRightMove(BoardState* boardState, enum PlayerType playerType, int offset)
{
  return NULL;
}



Moves* generateDiagonalDownLeftMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}



Moves* generateDiagonalDownLeftMove(BoardState* boardState, enum PlayerType playerType, int offset)
{
  return NULL;
}


/******************************************************************************************/




Moves* generateStepMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}




Moves* generateLMoves(BoardState* boardState, enum PlayerType playerType)
{
  return NULL;
}







