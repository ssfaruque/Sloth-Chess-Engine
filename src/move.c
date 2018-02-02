
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
            if(evaluation >= move.boardEval)
            {
              move = moves->quietMoves[moveNum];
              move.boardEval = evaluation;
            }
          }

          // minimize score
          else if(colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
          {
            if(evaluation <= move.boardEval)
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
            if(evaluation >= move.boardEval)
            {
              move = moves->quietMoves[moveNum];
              move.boardEval = evaluation;
            }
          }

          // minimize score
          else if(colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
          {
            if(evaluation <= move.boardEval)
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
  
  
  Moves* moves = (Moves*) malloc(sizeof(Moves));
  moves->captureMoves = (Move*) malloc(sizeof(Move) * 50);
  moves->quietMoves = (Move*) malloc(sizeof(Move) * 50);
  
  int moveNum = 0; //number of moves
  Move move;
  moves->numCaptureMoves = 0; // number of Capture moves
  moves->numQuietMoves = 0; // Number of Quiet moves
  // If White, only slide up
  if(colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
  {
    /*Generate Slide Up 1*/
    move = generateSlideUpMove(isolatedPiece, boardState, colorType,1);
    // If Valid and is not Capture, added to moves.
    if (move.movedPosition &&
        !(move.movedPosition & boardState->boards[!colorType]))
      moves->quietMoves[moves->numQuietMoves++] = move;
    /*Generate Slide Up 2 when unmoved*/
    if ((isolatedPiece & 0x000000000000ff00) != 0) //if in initial position
    {
      move = generateSlideUpMove(isolatedPiece, boardState, colorType, 2);
      // If Valid and is not Capture, added to moves.
      if (move.movedPosition &&
          !(move.movedPosition & boardState->boards[!colorType]))
        moves->quietMoves[moves->numQuietMoves++] = move;
    }
    
    //if Capture, has to be Diagonal
	  	
		  move = generateDiagonalUpLeftMove(isolatedPiece, boardState, colorType, 1);
		  if( move.movedPosition & boardState->boards[!colorType])
        moves->captureMoves[moves->numCaptureMoves++] = move;
		  move = generateDiagonalUpRightMove(isolatedPiece, boardState, colorType, 1);
		  if (move.movedPosition & boardState->boards[!colorType])
        moves->captureMoves[moves->numCaptureMoves++] = move;
    
    
    
  }
  
  // Black , only slide down
  else if(colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
  {
    /*Generate Slide Up 1*/
    move = generateSlideDownMove(isolatedPiece, boardState, colorType, 1);
    // If Valid and is not Capture, added to moves.
    if (move.movedPosition &&
        !(move.movedPosition & boardState->boards[!colorType]))
      moves->quietMoves[moves->numQuietMoves++] = move;
    /*Generate Slide Up 2 when unmoved*/
    if ((isolatedPiece & 0x00ff000000000000) != 0) //if in initial position
    {
      move = generateSlideDownMove(isolatedPiece, boardState, colorType, 2);
      // If Valid and is not Capture, added to moves
      if (move.movedPosition &&
          !(move.movedPosition & boardState->boards[!colorType]))
        moves->quietMoves[moves->numQuietMoves++] = move;
    }
	   //if Capture, has to be Diagonal
    
		  move = generateDiagonalDownLeftMove(isolatedPiece, boardState, colorType, 1);
		  if (move.movedPosition & boardState->boards[!colorType])
        moves->captureMoves[moves->numCaptureMoves++] = move;
		  move = generateDiagonalDownRightMove(isolatedPiece, boardState, colorType, 1);
		  if (move.movedPosition & boardState->boards[!colorType])
        moves->captureMoves[moves->numCaptureMoves++] = move;
    
    
  }
  // Need to implement en passant 
  return moves;
  
}


Moves* generateRookMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType)
{
  
  Moves* moves = (Moves*) malloc(sizeof(Moves));
  moves->captureMoves = (Move*) malloc(sizeof(Move) * 50);
  moves->quietMoves = (Move*) malloc(sizeof(Move) * 50);
  
  int moveNum = 0; //number of moves
  Move move;
  moves->numCaptureMoves = 0; // number of Capture moves
  moves->numQuietMoves = 0; // Number of Quiet moves
  
  //Slide Up Moves
  int i = 1; //for looping
  while (1)
  {
    
    move = generateSlideUpMove(isolatedPiece, boardState, colorType, i++);
    //if legal move
    if (move.movedPosition)
    {
      //capture move once in this direction
      if (move.movedPosition & boardState->boards[!colorType])
      {
        moves->captureMoves[moves->numCaptureMoves++] = move;
        break;
      }
      else//quiet
        moves->quietMoves[moves->numQuietMoves++] = move;
    }
    else
      break;
  }
  //Slide Down Moves
  i = 1;
  while (1)
  {
    
    move = generateSlideDownMove(isolatedPiece, boardState, colorType, i++);
    //if legal move
    if (move.movedPosition)
    {
      //capture move once in this direction
      if (move.movedPosition & boardState->boards[!colorType])
      {
        moves->captureMoves[moves->numCaptureMoves++] = move;
        break;
      }
      else//quiet
        moves->quietMoves[moves->numQuietMoves++] = move;
    }
    else
      break;
  }
  //Slide Left Moves
  i = 1;
  while (1)
  {
    
    move = generateSlideLeftMove(isolatedPiece, boardState, colorType, i++);
    //if legal move
    if (move.movedPosition)
    {
      //capture move once in this direction
      if (move.movedPosition & boardState->boards[!colorType])
      {
        moves->captureMoves[moves->numCaptureMoves++] = move;
        break;
      }
      else//quiet
        moves->quietMoves[moves->numQuietMoves++] = move;
    }
    else
      break;
  }
  //Slide Right Moves
  i = 1;
  while (1)
  {
    move = generateSlideRightMove(isolatedPiece, boardState, colorType, i++);
    //if legal move
    if (move.movedPosition)
    {
      //capture move once in this direction
      if (move.movedPosition & boardState->boards[!colorType])
      {
        moves->captureMoves[moves->numCaptureMoves++] = move;
        break;
      }
      else//quiet
        moves->quietMoves[moves->numQuietMoves++] = move;
    }
    else
      break;
  }
  
  
  
  
  return moves;
}


Moves* generateKnightMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType)
{
    Moves* moves = (Moves*) malloc(sizeof(Moves));
    moves->captureMoves = (Move*) malloc(sizeof(Move) * 50);
    moves->quietMoves = (Move*) malloc(sizeof(Move) * 50);
  
    int moveNum = 0;
    Move move;
    moves->numCaptureMoves = 0;
    moves->numQuietMoves = 0;

    move = generateUpUpLeft(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
        //capture move
        if (move.movedPosition & boardState->boards[!colorType])
            moves->captureMoves[moves->numCaptureMoves++] = move;
        else
            moves->quietMoves[moves->numQuietMoves++] = move;
    }


    move = generateUpLeftLeft(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
       //capture move
        if (move.movedPosition & boardState->boards[!colorType])
            moves->captureMoves[moves->numCaptureMoves++] = move;
        else
            moves->quietMoves[moves->numQuietMoves++] = move;
    }

    move = generateUpUpRight(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
        //capture move
        if (move.movedPosition & boardState->boards[!colorType])
            moves->captureMoves[moves->numCaptureMoves++] = move;
        else
            moves->quietMoves[moves->numQuietMoves++] = move;
    }

    move = generateUpRightRight(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
        //capture move
        if (move.movedPosition & boardState->boards[!colorType])
            moves->captureMoves[moves->numCaptureMoves++] = move;
        else
            moves->quietMoves[moves->numQuietMoves++] = move;

    move = generateDownDownLeft(isolatedPiece, boardState, colorType);
    }

        //if legal move
    if (move.movedPosition)
    {
        //capture move
        if (move.movedPosition & boardState->boards[!colorType])
            moves->captureMoves[moves->numCaptureMoves++] = move;
        else
            moves->quietMoves[moves->numQuietMoves++] = move;
    }

    move = generateDownLeftLeft(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
        //capture move
        if (move.movedPosition & boardState->boards[!colorType])
            moves->captureMoves[moves->numCaptureMoves++] = move;
        else
            moves->quietMoves[moves->numQuietMoves++] = move;
    }

    move = generateDownRightRight(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
       //capture move
        if (move.movedPosition & boardState->boards[!colorType])
            moves->captureMoves[moves->numCaptureMoves++] = move;
        else
            moves->quietMoves[moves->numQuietMoves++] = move;
    }

    move = generateDownDownRight(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
        //capture move
        if (move.movedPosition & boardState->boards[!colorType])
            moves->captureMoves[moves->numCaptureMoves++] = move;
        else
            moves->quietMoves[moves->numQuietMoves++] = move;
    }

    return moves;
}





Moves* generateBishopMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType)
{
  Moves* moves = (Moves*) malloc(sizeof(Moves));
  moves->captureMoves = (Move*) malloc(sizeof(Move) * 50);
  moves->quietMoves = (Move*) malloc(sizeof(Move) * 50);
  
    int moveNum = 0;
    Move move;
    moves->numCaptureMoves = 0;
    moves->numQuietMoves = 0;
    int i = 1;

    while(1)
    {

        move = generateDiagonalDownLeftMove(isolatedPiece, boardState, colorType, i++);
        //if legal move
        if (move.movedPosition)
        {
            //capture move
            if (move.movedPosition & boardState->boards[!colorType])
            {
                moves->captureMoves[moves->numCaptureMoves++] = move;
                break;
            }

            else
                moves->quietMoves[moves->numQuietMoves++] = move;
        }
        else
            break;
    }

    i = 1;
    while(1)
    {

        move = generateDiagonalDownRightMove(isolatedPiece, boardState, colorType, i++);
        //if legal move
        if (move.movedPosition)
        {
            //capture move
            if (move.movedPosition & boardState->boards[!colorType])
            {
                moves->captureMoves[moves->numCaptureMoves++] = move;
                break;
            }
            else
                moves->quietMoves[moves->numQuietMoves++] = move;
        }

        else
            break;
    }

    i = 1;
    while(1)
    {

        move = generateDiagonalUpLeftMove(isolatedPiece, boardState, colorType, i++);
        //if legal move
        if (move.movedPosition)
        {
            //capture move
            if (move.movedPosition & boardState->boards[!colorType])
            {
                moves->captureMoves[moves->numCaptureMoves++] = move;
                break;
            }
            else
                moves->quietMoves[moves->numQuietMoves++] = move;
        }

        else
            break;
    }

    i = 1;
    while(1)
    {

        move = generateDiagonalUpRightMove(isolatedPiece, boardState, colorType, i++);
        //if legal move
        if (move.movedPosition)
        {
            //capture move
            if (move.movedPosition & boardState->boards[!colorType])
            {
                moves->captureMoves[moves->numCaptureMoves++] = move;
                break;
            }
            else
                moves->quietMoves[moves->numQuietMoves++] = move;
        }

        else
            break;
    }

    return moves;
}



Moves* generateQueenMoves(BoardState* boardState,
                        Bitboard isolatedPiece,
                        enum BitboardType colorType)
{
  Moves* moveSet1 = generateRookMoves(boardState, isolatedPiece, colorType);
  Moves* moveSet2 = generateBishopMoves(boardState, isolatedPiece, colorType);
  Moves* moves = (Moves*) malloc(sizeof(Moves));
  moves->captureMoves = (Move*) malloc(sizeof(Move) * 50);
  moves->quietMoves = (Move*) malloc(sizeof(Move) * 50);
 
  moves->numCaptureMoves = moveSet1->numCaptureMoves + moveSet2->numCaptureMoves;
  moves->numQuietMoves   = moveSet1->numQuietMoves + moveSet2->numQuietMoves;
  
  
  int i;
  
  for(i = 0; i < moveSet1->numCaptureMoves; ++i)
    moves->captureMoves[i] = moveSet1->captureMoves[i];
  
  for(i = 0; i < moveSet2->numCaptureMoves; ++i)
    moves->captureMoves[i + moveSet1->numCaptureMoves] = moveSet2->captureMoves[i];
  
  for(i = 0; i < moveSet1->numQuietMoves; ++i)
    moves->quietMoves[i] = moveSet1->quietMoves[i];
  
  for(i = 0; i < moveSet2->numQuietMoves; ++i)
    moves->quietMoves[i + moveSet1->numQuietMoves] = moveSet2->quietMoves[i];
  
  return moves;
}





Moves* generateKingMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType)
{
  Moves* moves = (Moves*) malloc(sizeof(Moves));
  moves->captureMoves = (Move*) malloc(sizeof(Move) * 50);
  moves->quietMoves = (Move*) malloc(sizeof(Move) * 50);
  
  Move move = {0, 0, 0};
  moves->numCaptureMoves = 0;
  moves->numQuietMoves = 0;
  
  typedef Move (*kingMove)(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           int offset);
  
  kingMove kingMoveGenerate[] = {generateSlideLeftMove,       generateSlideRightMove,
                                 generateSlideUpMove,         generateSlideDownMove,
                                 generateDiagonalUpRightMove, generateDiagonalDownRightMove,
                                 generateDiagonalUpLeftMove,  generateDiagonalDownLeftMove};
  
  
  int i;
  
  for(i = 0; i < sizeof(kingMoveGenerate) / sizeof(kingMoveGenerate[0]); ++i)
  {
    move = kingMoveGenerate[i](isolatedPiece, boardState, colorType, 1);
    
    if (move.movedPosition)
    {
      //capture move
      if (move.movedPosition & boardState->boards[!colorType])
        moves->captureMoves[moves->numCaptureMoves++] = move;
      
      else
        moves->quietMoves[moves->numQuietMoves++] = move;
    }
  }
  
  return moves;
}


/******************************************************************************************/



/******************************** Moves pertaining to slides ********************************/



Move generateSlideLeftMove(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
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
                       enum BitboardType colorType
                       )
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
                          enum BitboardType colorType
                          )
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
                          enum BitboardType colorType
                          )
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
                           enum BitboardType colorType
                           )
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
                          enum BitboardType colorType
                          )
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
                          enum BitboardType colorType
                          )
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
                        enum BitboardType colorType
                        )
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
                      enum BitboardType colorType
                      )
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





