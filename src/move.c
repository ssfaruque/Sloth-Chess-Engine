
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * Description of Chess Engine HERE                                             *
 ********************************************************************************/


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



void updateBoardState(BoardState* boardState,
                      Bitboard initialPiece,
                      Bitboard movedPiece,
                      enum BitboardType colorType,
                      enum BitboardType pieceType)
{
  // only quiet moves for now
  boardState->boards[colorType] = ((boardState->boards[colorType] ^ initialPiece) | movedPiece);
  boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ initialPiece) | movedPiece);
  
}



/******************************** Generate moves by pieces ********************************/


Move generateMove(BoardState* boardState,
                  enum BitboardType colorType,
                  enum BitboardType pieceType)
{
  Move move;
  return move;
}




Move generatePawnMove(BoardState* boardState, enum BitboardType colorType)
{
  Move move;
  return move;
}


Move generateRookMove(BoardState* boardState, enum BitboardType colorType)
{
  Move move;
  return move;
}


Move generateKnightMove(BoardState* boardState, enum BitboardType colorType)
{
  Move move;
  return move;
}



Move generateBishopMove(BoardState* boardState, enum BitboardType colorType)
{
  Move move;
  return move;
}



Move generateQueenMove(BoardState* boardState, enum BitboardType colorType)
{
  Move move;
  return move;
}



Move generateKingMove(BoardState* boardState, enum BitboardType colorType)
{
  Move move;
  return move;
}


/******************************************************************************************/



/******************************** Moves pertaining to slides ********************************/



Move generateSlideLeftMove(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           enum BitboardType pieceType,
                           int offset)
{
  /*
  Bitboard movedPiece = initialPosition <<  offset;
  Bitboard collisions = movedPiece & boardState->boards[colorType];
 
  // check for collision with own piece and out of bounds conditions
  if(!collisions && !(movedPiece & 0x0101010101010101))
  {
    return movedPiece;
  }*/
  
  Move move;
  return move;
}




Move generateSlideRightMove(Bitboard initialPosition,
                            BoardState* boardState,
                            enum BitboardType colorType,
                            enum BitboardType pieceType,
                            int offset)
{
  /*
  Bitboard movedPiece = initialPosition >>  offset;
  Bitboard collisions = movedPiece & boardState->boards[colorType];
  
  // check for collision with own piece and out of bounds conditions
  if(!collisions && !(movedPiece & 0x8080808080808080))
  {
    return movedPiece;
  }*/
  
  Move move;
  return move;
}





Move generateSlideUpMove(Bitboard initialPosition,
                         BoardState* boardState,
                         enum BitboardType colorType,
                         enum BitboardType pieceType,
                         int offset)
{
  /*
  Bitboard movedPiece = initialPosition << (8 * offset);
  Bitboard collisions = movedPiece & boardState->boards[colorType];
  
  // check for collision with own piece and out of bounds conditions
  if(!collisions && movedPiece)
  {
    return movedPiece;
  }*/
  
  Move move;
  return move;
}




Move generateSlideDownMove(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           enum BitboardType pieceType,
                           int offset)
{
  /*
  Bitboard movedPiece = initialPosition >> (8 * offset);
  Bitboard collisions = movedPiece & boardState->boards[colorType];
  
  // check for collision with own piece and out of bounds conditions
  if(!collisions && movedPiece)
  {
    return movedPiece;
  }*/
  
  Move move;
  return move;
}


/******************************************************************************************/




/******************************** Moves pertaining to diagonals ********************************/



Move generateDiagonalUpRightMove(Bitboard initialPosition,
                                   BoardState* boardState,
                                   enum BitboardType colorType,
                                   enum BitboardType pieceType,
                                   int offset)
{
  Move move;
  return move;
}




Move generateDiagonalUpLeftMove(Bitboard initialPosition,
                                  BoardState* boardState,
                                  enum BitboardType colorType,
                                  enum BitboardType pieceType,
                                  int offset)
{
  Move move;
  return move;
}





Move generateDiagonalDownRightMove(Bitboard initialPosition,
                                     BoardState* boardState,
                                     enum BitboardType colorType,
                                     enum BitboardType pieceType,
                                     int offset)
{
  Move move;
  return move;
}




Move generateDiagonalDownLeftMove(Bitboard initialPosition,
                                    BoardState* boardState,
                                    enum BitboardType colorType,
                                    enum BitboardType pieceType,
                                    int offset)
{
  Move move;
  return move;
}


/******************************************************************************************/









Move generateLMove(Bitboard initialPosition,
                    BoardState* boardState,
                    enum BitboardType colorType,
                    enum BitboardType pieceType,
                    int offset)
{
  Move move;
  return move;
}







