
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
#include "eval.h"


typedef Moves* (*moveGeneration)(BoardState* boardState,
                               Bitboard isolatedPiece,
                               enum BitboardType colorType);

moveGeneration possibleMoves[] = {generatePawnMoves,   generateRookMoves,
                                  generateKnightMoves, generateBishopMoves,
                                  generateQueenMoves,  generateKingMoves};



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
                  int recurseDepth)
{
  Move move = {0, 0, 0};
  
  int i;
  
  for(i = 0; i < NUM_PIECES; ++i)
  {
    int pieceType = i + 2;
    
    Bitboard piecesBoard = (boardState->boards[colorType] & boardState->boards[pieceType]);
    
    while(piecesBoard)
    {
      Bitboard isolatedPiece = piecesBoard & -piecesBoard;
      
      Moves* moves = possibleMoves[i](boardState, isolatedPiece, colorType);
      
      int moveNum;
      
      for(moveNum = 0; moveNum < moves->numQuietMoves; ++moveNum)
      {
        updateBoardState(boardState, moves->quietMoves[moveNum].initialPosition,
                         moves->quietMoves[moveNum].movedPosition, colorType, pieceType);
        
        
        if(recurseDepth == MAX_RECURSION_DEPTH)
        {
          int evaluation = eval(boardState);
          
          // maximize score
          if(colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
          {
            if(evaluation > move.boardEval)
            {
              move = moves->quietMoves[moveNum];
              move.boardEval = evaluation;
            }
          }
          
          // minimize score
          else if(colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
          {
            if(evaluation < move.boardEval)
            {
              move = moves->quietMoves[moveNum];
              move.boardEval = evaluation;
            }
          }
          
          updateBoardState(boardState, moves->quietMoves[moveNum].movedPosition,
                           moves->quietMoves[moveNum].initialPosition, colorType, pieceType);
          
          continue;
        }
        
        move = generateMove(boardState, !colorType, recurseDepth + 1);
        
        updateBoardState(boardState, moves->quietMoves[moveNum].movedPosition,
                         moves->quietMoves[moveNum].initialPosition, colorType, pieceType);
      }
      
      
      for(moveNum = 0; moveNum < moves->numCaptureMoves; ++moveNum)
      {
        updateBoardState(boardState, moves->captureMoves[moveNum].initialPosition,
                         moves->captureMoves[moveNum].movedPosition, colorType, pieceType);
        
        
        if(recurseDepth == MAX_RECURSION_DEPTH)
        {
          int evaluation = eval(boardState);
          
          // maximize score
          if(colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
          {
            if(evaluation > move.boardEval)
            {
              move = moves->quietMoves[moveNum];
              move.boardEval = evaluation;
            }
          }
          
          // minimize score
          else if(colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
          {
            if(evaluation < move.boardEval)
            {
              move = moves->quietMoves[moveNum];
              move.boardEval = evaluation;
            }
          }
          
          updateBoardState(boardState, moves->captureMoves[moveNum].movedPosition,
                           moves->captureMoves[moveNum].initialPosition, colorType, pieceType);
          
          continue;
        }
        
        move = generateMove(boardState, !colorType, recurseDepth + 1);
        
        updateBoardState(boardState, moves->captureMoves[moveNum].movedPosition,
                         moves->captureMoves[moveNum].initialPosition, colorType, pieceType);
      }
      
   
      // done with moves, free the possible moves
    
      
      piecesBoard &= piecesBoard - 1; // reset ls1b
    }
   
  }
  
  return move;
}



Moves* generatePawnMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType)
{
  // en passant, and move 2 spaces for first turn later on
  
  Moves* moves = (Moves*) malloc(sizeof(Moves) * 50);
  int moveNum = 0;
  
  
  if(colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
  {
    
  }
  
  
  else if(colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
  {
    
  }
  
  return moves;
}


Moves* generateRookMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType)
{
  return NULL;
}


Moves* generateKnightMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType)
{
  return NULL;
}



Moves* generateBishopMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType)
{
  return NULL;
}



Moves* generateQueenMoves(BoardState* boardState,
                        Bitboard isolatedPiece,
                        enum BitboardType colorType)
{
  return NULL;
}



Moves* generateKingMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType)
{

  return NULL;
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


/******************************** Moves pertaining to L moves ********************************/



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


/******************************************************************************************/



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


int isKingInCheck(BoardState* boardState, enum BitboardType colorType)
{
  
  return 0;
}





