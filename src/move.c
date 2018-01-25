
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
  Move move = {initialPosition, 0};
  int col = findCol(initialPosition);
  
  // if not out of bounds
  if(!(offset > col - 1))
  {
    move.movedPosition = initialPosition << offset;
    
    if(move.movedPosition & boardState->boards[colorType])
      move.movedPosition = 0;
  }
  
  return move;
}


Move generateSlideRightMove(Bitboard initialPosition,
                            BoardState* boardState,
                            enum BitboardType colorType,
                            enum BitboardType pieceType,
                            int offset)
{
  Move move = {initialPosition, 0};
  int col = findCol(initialPosition);
  
  // if not out of bounds
  if(!(offset > BOARD_LENGTH - col))
  {
    move.movedPosition = initialPosition >> offset;
    
    if(move.movedPosition & boardState->boards[colorType])
      move.movedPosition = 0;
  }
  
  return move;
}





Move generateSlideUpMove(Bitboard initialPosition,
                         BoardState* boardState,
                         enum BitboardType colorType,
                         enum BitboardType pieceType,
                         int offset)
{
  Move move = {initialPosition, 0};
  move.movedPosition  = move.movedPosition << (8 * offset);
  
  // if not out of bounds and does not collide with own pieces
  if(move.movedPosition != 0 && !(move.movedPosition & boardState->boards[colorType]))
    return move;
  
  move.movedPosition = 0;
  return move;
}


Move generateSlideDownMove(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           enum BitboardType pieceType,
                           int offset)
{
  Move move = {initialPosition, 0};
  move.movedPosition  = move.movedPosition >> (8 * offset);
  
  // if not out of bounds and does not collide with own pieces
  if(move.movedPosition != 0 && !(move.movedPosition & boardState->boards[colorType]))
    return move;
  
  move.movedPosition = 0;
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
  Move move = {initialPosition, 0};
  int col = findCol(initialPosition);
  
  // if not out of bounds horizontally
  if(!(offset > BOARD_LENGTH - col))
  {
    move.movedPosition = initialPosition << (7 * offset);
    
    if(move.movedPosition & boardState->boards[colorType])
      move.movedPosition = 0;
  }
  
  return move;
}




Move generateDiagonalUpLeftMove(Bitboard initialPosition,
                                  BoardState* boardState,
                                  enum BitboardType colorType,
                                  enum BitboardType pieceType,
                                  int offset)
{
  Move move = {initialPosition, 0};
  int col = findCol(initialPosition);
  
  // if not out of bounds horizontally
  if(!(offset > col - 1))
  {
    move.movedPosition = initialPosition << (9 * offset);
    
    if(move.movedPosition & boardState->boards[colorType])
      move.movedPosition = 0;
  }
  
  return move;
}





Move generateDiagonalDownRightMove(Bitboard initialPosition,
                                     BoardState* boardState,
                                     enum BitboardType colorType,
                                     enum BitboardType pieceType,
                                     int offset)
{
  Move move = {initialPosition, 0};
  int col = findCol(initialPosition);
  
  // if not out of bounds horizontally
  if(!(offset > BOARD_LENGTH - col))
  {
    move.movedPosition = initialPosition >> (7 * offset);
    
    if(move.movedPosition & boardState->boards[colorType])
      move.movedPosition = 0;
  }
  
  return move;
}




Move generateDiagonalDownLeftMove(Bitboard initialPosition,
                                    BoardState* boardState,
                                    enum BitboardType colorType,
                                    enum BitboardType pieceType,
                                    int offset)
{
  Move move = {initialPosition, 0};
  int col = findCol(initialPosition);
  
  // if not out of bounds horizontally
  if(!(offset > col - 1))
  {
    move.movedPosition = initialPosition >> (9 * offset);
    
    if(move.movedPosition & boardState->boards[colorType])
      move.movedPosition = 0;
  }
  
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



Move generateUpUpRight(Bitboard initialPosition,
                       BoardState* boardState,
                       enum BitboardType colorType,
                       enum BitboardType pieceType)
{
  Move move = {initialPosition, 0};
  
  if(initialPosition & 0x0101010101010101)
    move.movedPosition = 0;
  
  else
    move.movedPosition = initialPosition << 15;
  
  if(move.movedPosition & boardState->boards[colorType])
    move.movedPosition = 0;
  
  return move;
}


Move generateUpRightRight(Bitboard initialPosition,
                          BoardState* boardState,
                          enum BitboardType colorType,
                          enum BitboardType pieceType)
{
  Move move = {initialPosition, 0};
  
  if(initialPosition & 0x0303030303030303)
    move.movedPosition = 0;
  
  else
    move.movedPosition = initialPosition << 6;
  
  if(move.movedPosition & boardState->boards[colorType])
    move.movedPosition = 0;
  
  return move;
}


Move generateDownRightRight(Bitboard initialPosition,
                          BoardState* boardState,
                          enum BitboardType colorType,
                          enum BitboardType pieceType)
{
  Move move = {initialPosition, 0};
  
  if(initialPosition & 0x0303030303030303)
    move.movedPosition = 0;
  
  else
    move.movedPosition = initialPosition >> 10;
  
  if(move.movedPosition & boardState->boards[colorType])
    move.movedPosition = 0;
  
  return move;
}




Move generateDownDownRight(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           enum BitboardType pieceType)
{
  Move move = {initialPosition, 0};
  
  if(initialPosition & 0x0101010101010101)
    move.movedPosition = 0;
  
  else
    move.movedPosition = initialPosition >> 17;
  
  if(move.movedPosition & boardState->boards[colorType])
    move.movedPosition = 0;
  
  return move;
}



Move generateDownDownLeft(Bitboard initialPosition,
                          BoardState* boardState,
                          enum BitboardType colorType,
                          enum BitboardType pieceType)
{
  Move move = {initialPosition, 0};
  
  if(initialPosition & 0x8080808080808080)
    move.movedPosition = 0;
  
  else
    move.movedPosition = initialPosition >> 15;
  
  if(move.movedPosition & boardState->boards[colorType])
    move.movedPosition = 0;
  
  return move;
}


Move generateDownLeftLeft(Bitboard initialPosition,
                          BoardState* boardState,
                          enum BitboardType colorType,
                          enum BitboardType pieceType)
{
  Move move = {initialPosition, 0};
  
  if(initialPosition & 0xc0c0c0c0c0c0c0c0)
    move.movedPosition = 0;
  
  else
    move.movedPosition = initialPosition >> 6;
  
  if(move.movedPosition & boardState->boards[colorType])
    move.movedPosition = 0;
  
  return move;
}


Move generateUpLeftLeft(Bitboard initialPosition,
                        BoardState* boardState,
                        enum BitboardType colorType,
                        enum BitboardType pieceType)
{
  Move move = {initialPosition, 0};
  
  if(initialPosition & 0xc0c0c0c0c0c0c0c0)
    move.movedPosition = 0;
  
  else
    move.movedPosition = initialPosition << 10;
  
  if(move.movedPosition & boardState->boards[colorType])
    move.movedPosition = 0;
  
  return move;
}



Move generateUpUpLeft(Bitboard initialPosition,
                      BoardState* boardState,
                      enum BitboardType colorType,
                      enum BitboardType pieceType)
{
  Move move = {initialPosition, 0};
  
  if(initialPosition & 0x8080808080808080)
    move.movedPosition = 0;
  
  else
    move.movedPosition = initialPosition << 17;
  
  if(move.movedPosition & boardState->boards[colorType])
    move.movedPosition = 0;
  
  return move;
}











int findCol(Bitboard initialPosition)
{
  // if piece is in left half of the board
  if(initialPosition & 0xf0f0f0f0f0f0f0f0)
  {
    // if 2 left most columns
    if(initialPosition & 0xc0c0c0c0c0c0c0c0)
    {
      // if left most column
      if(initialPosition & 0x8080808080808080)
        return 1;
      
      // else in 2nd column from left
      else
        return 2;
    }
    
    // else in 3rd and 4th columns from the left
    else
    {
      // if in 3rd column from left
      if(initialPosition & 0x2020202020202020)
        return 3;
      
      // else in 4th column from left
      else
        return 4;
    }
  }
  
  // else piece is in right hand of the board
  else
  {
    // if in 2 right most columns
    if(initialPosition & 0x0303030303030303)
    {
      if(initialPosition & 0x0202020202020202)
        return 7;
      
      else
        return 8;
    }
    
    // else in 3rd and 4th columns from the right
    else
    {
      if(initialPosition & 0x0808080808080808)
        return 5;
      
      else
        return 6;
    }
  }
}





