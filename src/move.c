
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * Description of Chess Engine HERE                                             *
 ********************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "move.h"
#include "eval.h"


typedef Move* (*moveGeneration)(BoardState* boardState,
                                Bitboard isolatedPiece,
                                enum BitboardType colorType);

moveGeneration possibleMoves[] = {generatePawnMoves,   generateRookMoves,
                                  generateKnightMoves, generateBishopMoves,
                                  generateQueenMoves,  generateKingMoves};




void updateFlagState(BoardState* boardState,
                      Bitboard initialPiece,
                      Bitboard movedPiece,
                      enum BitboardType colorType,
                      enum BitboardType pieceType,
                      enum MoveType moveType)
{


    if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS && initialPiece & 0x00ff00000000000 &&
            movedPiece & 0x000000ff00000000)
    {
        boardState->flagState = boardState->flagState | (movedPiece << 8);
    }

    if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS && initialPiece & 0x00000000000ff00 &&
            movedPiece & 0x00000000ff000000)
    {
        boardState->flagState = boardState->flagState | (movedPiece >> 8);
    }



}

int updateBoardState(BoardState* boardState,
                      Bitboard initialPiece,
                      Bitboard movedPiece,
                      enum BitboardType colorType,
                      enum BitboardType pieceType,
                      enum MoveType moveType,
                      int capturedPiece,
                      int undo)
{
      if(undo && !capturedPiece) //undo quiet
      {
        boardState->boards[colorType] = ((boardState->boards[colorType] ^ movedPiece) | initialPiece);
        boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ movedPiece) | initialPiece);
      }
  
      else if(!capturedPiece) // quiet
      {
  
        boardState->boards[colorType] = ((boardState->boards[colorType] ^ initialPiece) | movedPiece);
        boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ initialPiece) | movedPiece);
      }
  
      else if (capturedPiece && undo) // undo capture
      {
        boardState->boards[colorType] = ((boardState->boards[colorType] ^ movedPiece) | initialPiece);
        boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ movedPiece) | initialPiece);
        
          boardState->boards[capturedPiece] = (boardState->boards[capturedPiece] | movedPiece);
          boardState->boards[!colorType] = ((boardState->boards[!colorType]) ^ movedPiece);
      }
  
      else if(capturedPiece) //capture
      {
        
          boardState->boards[capturedPiece] = ((boardState->boards[capturedPiece]) | movedPiece);
          boardState->boards[!colorType] = ((boardState->boards[!colorType]) ^ movedPiece);
        
          boardState->boards[colorType] = ((boardState->boards[colorType] ^ initialPiece) | movedPiece);
          boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ initialPiece) | movedPiece);
        
      }
       
  
  
  return 0;
}



/******************************** Generate moves by pieces ********************************/



int negaMax(BoardState* boardState,
            enum BitboardType colorType,
            int depth)
{
  if (depth == 0)
    return eval(boardState);
  
  int max = -111111111;
  
  Move* moves = generateAllMoves(boardState, colorType);
  int numMoves = 950;
  int moveNum = 0;
  int score = 0;
  
  for(moveNum = 0; moveNum < numMoves; ++moveNum)
  {
    if(moves[moveNum].initialPosition)
    {
      updateBoardState(boardState, moves[moveNum].initialPosition, moves[moveNum].movedPosition, colorType, moves[moveNum].pieceType, 0, moves[moveNum].capturedPiece, 0);
      
      score = -negaMax(boardState, !colorType, depth - 1);
      
      updateBoardState(boardState, moves[moveNum].initialPosition, moves[moveNum].movedPosition, colorType, moves[moveNum].pieceType, 0, moves[moveNum].capturedPiece, 1);
      
      if(score > max)
        max = score;
        
      }
  }

  return max;
}




Move generateMove(BoardState* boardState,
                  enum BitboardType colorType,
                  int recurseDepth)
{
  Move move = {0, 0, 0, 0, 0};
  
  int numFirstMoves = 950;
  int i;
  
  int score = 0;
  int maxScore = 0;
  
  Move* firstMoves = generateAllMoves(boardState, colorType);
  
  for(i = 0; i < numFirstMoves; ++i)
  {
    
    if(firstMoves[i].initialPosition)
    {
      // do the move
      updateBoardState(boardState, firstMoves[i].initialPosition, firstMoves[i].movedPosition, colorType, firstMoves[i].pieceType, 0, firstMoves[i].capturedPiece, 0);
      
      score = negaMax(boardState, !colorType, recurseDepth - 1);
      
      // undo the move
      updateBoardState(boardState, firstMoves[i].initialPosition, firstMoves[i].movedPosition, colorType, firstMoves[i].pieceType, 0, firstMoves[i].capturedPiece, 1);
      
      
      
      if(score >= maxScore)
      {
        maxScore = score;
        move = firstMoves[i];
      }
      
      
      
      /*if(colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
      {
        if(score > maxScore)
        {
          maxScore = score;
          move = firstMoves[i];
        }
      }
      
      else if(colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
      {
        if(score < maxScore)
        {
          maxScore = score;
          move = firstMoves[i];
        }
      }*/
      
    }
  
  }
  
  return move;
}




Move* generateAllMoves(BoardState* boardState,
                       enum BitboardType colorType)
{
  Move* moves = (Move*) malloc(sizeof(Move) * 950);
  
  Move* allPawnMoves = generateAllPawnMoves(boardState, colorType);
  Move* allRookMoves = generateAllRookMoves(boardState, colorType);
  Move* allKnightMoves = generateAllKnightMoves(boardState, colorType);
  Move* allBishopMoves = generateAllBishopMoves(boardState, colorType);
  Move* allQueenMoves = generateAllQueenMoves(boardState, colorType);
  Move* allKingMoves = generateAllKingMoves(boardState, colorType);
  
  
  memcpy((void*)&moves[0], (void*)allPawnMoves, sizeof(Move) * 400);
  memcpy((void*)&moves[400], (void*)allRookMoves, sizeof(Move) * 100);
  memcpy((void*)&moves[500], (void*)allKnightMoves, sizeof(Move) * 100);
  memcpy((void*)&moves[600], (void*)allBishopMoves, sizeof(Move) * 100);
  memcpy((void*)&moves[700], (void*)allQueenMoves, sizeof(Move) * 200);
  memcpy((void*)&moves[900], (void*)allKingMoves, sizeof(Move) * 50);
  
  return moves;
}




Move* generateAllPawnMoves(BoardState* boardState,
                            enum BitboardType colorType)
{
  Move* moves = (Move*) malloc(sizeof(Move) * 400);
  int pieceNum = 0;
  
  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_PAWN_POSITIONS] & boardState->boards[colorType];
  
  int i;
  
  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;
    Move* setOfMoves = generatePawnMoves(boardState, isolatedPiece, colorType);

    
    
    memcpy((void*)&moves[pieceNum++ * 50], (void*)setOfMoves, sizeof(Move) * 50);
    
    // reset ls1b
    pieces &= pieces - 1;
  }
  
  
  for(i = 0; i < 400; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_PAWN_POSITIONS;
  }
  
  return moves;
}



Move* generatePawnMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType)
{
  int i = 0; //For captured Piece type
  Move* moves = (Move*) calloc(50, sizeof(Move) * 50);
  int moveNum = 0;
  
  Move move;
  // If White, only slide up
  if(colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
  {
    /*Generate Slide Up 1*/
    move = generateSlideUpMove(isolatedPiece, boardState, colorType,1);
    
    // If Valid and is not Capture, added to moves.
    if(move.movedPosition &&
        !(move.movedPosition & boardState->boards[!colorType]))
      moves[moveNum++] = move;
    
    /*Generate Slide Up 2 when unmoved*/
    if((isolatedPiece & 0x000000000000ff00) != 0) // if in initial position
    {
      move = generateSlideUpMove(isolatedPiece, boardState, colorType, 2);
      // If Valid and is not Capture, added to moves.
      if(move.movedPosition &&
          !(move.movedPosition & boardState->boards[!colorType]))
        moves[moveNum++] = move;
        
    }

    //if Capture, has to be Diagonal

		  move = generateDiagonalUpLeftMove(isolatedPiece, boardState, colorType, 1);
		  if (move.movedPosition & boardState->boards[!colorType])
		  {
			  for (i = 0; i < NUM_PIECES; i++) // put captured Piece type into move
			  {
				  if (move.movedPosition & boardState->boards[i + 2])
				  {
					  move.capturedPiece = i + 2;
					  break;
				  }
			  }
        moves[moveNum++] = move;

		  }
    
		  move = generateDiagonalUpRightMove(isolatedPiece, boardState, colorType, 1);
		  if (move.movedPosition & boardState->boards[!colorType])
		  {
			  for (i = 0; i < NUM_PIECES; i++) // put captured Piece type into move
			  {
				  if (move.movedPosition & boardState->boards[i + 2])
				  {
					  move.capturedPiece = i + 2;
					  break;
				  }
			  }
			  moves[moveNum++] = move;

		  }

  }

  // Black , only slide down
  else if(colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
  {
    /*Generate Slide Up 1*/
    move = generateSlideDownMove(isolatedPiece, boardState, colorType, 1);
    
    // If Valid and is not Capture, added to moves.
    if (move.movedPosition &&
        !(move.movedPosition & boardState->boards[!colorType]))
      moves[moveNum++] = move;
    
    /*Generate Slide Up 2 when unmoved*/
    if ((isolatedPiece & 0x00ff000000000000) != 0) //if in initial position
    {
      move = generateSlideDownMove(isolatedPiece, boardState, colorType, 2);
      
      // If Valid and is not Capture, added to moves
      if (move.movedPosition &&
          !(move.movedPosition & boardState->boards[!colorType]))
        moves[moveNum++] = move;
    }
	   //if Capture, has to be Diagonal

		  move = generateDiagonalDownLeftMove(isolatedPiece, boardState, colorType, 1);
		  if (move.movedPosition & boardState->boards[!colorType])
		  {
			  for (i = 0; i < NUM_PIECES; i++) // put captured Piece type into move
			  {
				  if (move.movedPosition & boardState->boards[i + 2])
				  {
					  move.capturedPiece = i + 2;
					  break;
				  }
			  }
			  moves[moveNum++] = move;
			
		  }
		  move = generateDiagonalDownRightMove(isolatedPiece, boardState, colorType, 1);
		  if (move.movedPosition & boardState->boards[!colorType])
		  {
			  for (i = 0; i < NUM_PIECES; i++) // put captured Piece type into move
			  {
				  if (move.movedPosition & boardState->boards[i + 2])
				  {
					  move.capturedPiece = i + 2;
					  break;
				  }
			  }
			  moves[moveNum++] = move;

		  }
      


  }
  
  // Need to implement en passant
  return moves;

}



Move* generateAllRookMoves(BoardState* boardState,
                           enum BitboardType colorType)
{
  Move* moves = (Move*) malloc(sizeof(Move) * 100);
  int pieceNum = 0;
  
  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_ROOK_POSITIONS] & boardState->boards[colorType];
  
  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;
    Move* setOfMoves = generateRookMoves(boardState, isolatedPiece, colorType);
    
    memcpy((void*)&moves[pieceNum++ * 50], (void*)setOfMoves, sizeof(Move) * 50);
    
    // reset ls1b
    pieces &= pieces - 1;
  }
  
  
  int i;
  
  for(i = 0; i < 100; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_ROOK_POSITIONS;
  }
  
  return moves;
}




Move* generateRookMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType)
{

  Move* moves = (Move*) calloc(50, sizeof(Move) * 50);

  int j; //for captured Piece type
  int moveNum = 0; //number of moves
  Move move;


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
		  for (j = 0; j < NUM_PIECES; j++) // put captured Piece type into move
		  {
			  if (move.movedPosition & boardState->boards[j + 2])
			  {
				  move.capturedPiece = j + 2;
				  break;
			  }
		  }
		  moves[moveNum++] = move;
        break;
      }
      else//quiet
        moves[moveNum++] = move;
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
		  for (j = 0; j < NUM_PIECES; j++) // put captured Piece type into move
		  {
			  if (move.movedPosition & boardState->boards[j + 2])
			  {
				  move.capturedPiece = j + 2;
				  break;
			  }
		  }
		  moves[moveNum++] = move;
        break;
      }
      else//quiet
        moves[moveNum++] = move;
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
		   for (j = 0; j < NUM_PIECES; j++) // put captured Piece type into move
		  {
			  if (move.movedPosition & boardState->boards[j + 2])
			  {
				  move.capturedPiece = j + 2;
				  break;
			  }
		  }
		  moves[moveNum++] = move;
        break;
      }
      else//quiet
        moves[moveNum++] = move;
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
		  for (j = 0; j < NUM_PIECES; j++) // put captured Piece type into move
		  {
			  if (move.movedPosition & boardState->boards[j + 2])
			  {
				  move.capturedPiece = j + 2;
				  break;
			  }
		  }
		  moves[moveNum++] = move;
        break;
      }
      else//quiet
        moves[moveNum++] = move;
    }
    else
      break;
  }




  return moves;
}


Move* generateAllKnightMoves(BoardState* boardState,
                           enum BitboardType colorType)
{
  Move* moves = (Move*) malloc(sizeof(Move) * 100);
  int pieceNum = 0;
  
  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_KNIGHT_POSITIONS] & boardState->boards[colorType];
  
  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;
    Move* setOfMoves = generateKnightMoves(boardState, isolatedPiece, colorType);
    
    memcpy((void*)&moves[pieceNum++ * 50], (void*)setOfMoves, sizeof(Move) * 50);
    
    // reset ls1b
    pieces &= pieces - 1;
  }
  
  int i;
  
  for(i = 0; i < 100; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_KNIGHT_POSITIONS;
  }
  
  
  return moves;
}




Move* generateKnightMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType)
{
    Move* moves = (Move*) calloc(50, sizeof(Move) * 50);

	int i = 0;
  int moveNum = 0;
  Move move;


    move = generateUpUpLeft(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
        //capture move
        if (move.movedPosition & boardState->boards[!colorType])
		{
			for (i = 0; i < NUM_PIECES; i++) // put captured Piece type into move
			{
				if (move.movedPosition & boardState->boards[i + 2])
				{
					move.capturedPiece = i + 2;
					break;
				}
			}
			moves[moveNum++] = move;
		}
            
        else
            moves[moveNum++] = move;
    }


    move = generateUpLeftLeft(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
       //capture move
        if (move.movedPosition & boardState->boards[!colorType])
		{
			for (i = 0; i < NUM_PIECES; i++) // put captured Piece type into move
			{
				if (move.movedPosition & boardState->boards[i + 2])
				{
					move.capturedPiece = i + 2;
					break;
				}
			}
			moves[moveNum++] = move;
		}
        else
            moves[moveNum++] = move;
    }

    move = generateUpUpRight(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
        //capture move
        if (move.movedPosition & boardState->boards[!colorType])
		{
			for (i = 0; i < NUM_PIECES; i++) // put captured Piece type into move
			{
				if (move.movedPosition & boardState->boards[i + 2])
				{
					move.capturedPiece = i + 2;
					break;
				}
			}
			moves[moveNum++] = move;
		}
          
        else
            moves[moveNum++] = move;
    }

    move = generateUpRightRight(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
        //capture move
        if (move.movedPosition & boardState->boards[!colorType])
            moves[moveNum++] = move;
        else
            moves[moveNum++] = move;

    move = generateDownDownLeft(isolatedPiece, boardState, colorType);
    }

        //if legal move
    if (move.movedPosition)
    {
        //capture move
        if (move.movedPosition & boardState->boards[!colorType])
		{
			for (i = 0; i < NUM_PIECES; i++) // put captured Piece type into move
			{
				if (move.movedPosition & boardState->boards[i + 2])
				{
					move.capturedPiece = i + 2;
					break;
				}
			}
			moves[moveNum++] = move;
		}
        
        else
            moves[moveNum++] = move;
    }

    move = generateDownLeftLeft(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
        //capture move
		if (move.movedPosition & boardState->boards[!colorType])
		{
			for (i = 0; i < NUM_PIECES; i++) // put captured Piece type into move
			{
				if (move.movedPosition & boardState->boards[i + 2])
				{
					move.capturedPiece = i + 2;
					break;
				}
			}
			moves[moveNum++] = move;
		}
        
        else
            moves[moveNum++] = move;
    }

    move = generateDownRightRight(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
       //capture move
        if (move.movedPosition & boardState->boards[!colorType])
		{
			for (i = 0; i < NUM_PIECES; i++) // put captured Piece type into move
			{
				if (move.movedPosition & boardState->boards[i + 2])
				{
					move.capturedPiece = i + 2;
					break;
				}
			}
			moves[moveNum++] = move;
		}
        else
            moves[moveNum++] = move;
    }

    move = generateDownDownRight(isolatedPiece, boardState, colorType);
        //if legal move
    if (move.movedPosition)
    {
        //capture move
		if (move.movedPosition & boardState->boards[!colorType])
		{
			for (i = 0; i < NUM_PIECES; i++) // put captured Piece type into move
			{
				if (move.movedPosition & boardState->boards[i + 2])
				{
					move.capturedPiece = i + 2;
					break;
				}
			}
			moves[moveNum++] = move;
		}
            
        else
            moves[moveNum++] = move;
    }

    return moves;
}



Move* generateAllBishopMoves(BoardState* boardState,
                           enum BitboardType colorType)
{
  Move* moves = (Move*) malloc(sizeof(Move) * 100);
  int pieceNum = 0;
  
  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_BISHOP_POSITIONS] & boardState->boards[colorType];
  
  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;
    Move* setOfMoves = generateBishopMoves(boardState, isolatedPiece, colorType);
    
    memcpy((void*)&moves[pieceNum++ * 50], (void*)setOfMoves, sizeof(Move) * 50);
    
    // reset ls1b
    pieces &= pieces - 1;
  }
  
  
  int i;
  
  for(i = 0; i < 100; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_BISHOP_POSITIONS;
  }
  
  return moves;
}




Move* generateBishopMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType)
{
  Move* moves = (Move*) calloc(50, sizeof(Move) * 50);

  
    int moveNum = 0;
    Move move;

    int i = 1;//for looping
    int j = 0;//for captured Piece type

    while(1)
    {

        move = generateDiagonalDownLeftMove(isolatedPiece, boardState, colorType, i++);
        //if legal move
        if (move.movedPosition)
        {
            //capture move
            if (move.movedPosition & boardState->boards[!colorType])
            {
				for (j = 0; j < NUM_PIECES; j++) // put captured Piece type into move
				{
					if (move.movedPosition & boardState->boards[j + 2])
					{
						move.capturedPiece = j + 2;
						break;
					}
				}
				moves[moveNum++] = move;
                break;
            }

            else
                moves[moveNum++] = move;
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
				for (j = 0; j < NUM_PIECES; j++) // put captured Piece type into move
				{
					if (move.movedPosition & boardState->boards[j + 2])
					{
						move.capturedPiece = j + 2;
						break;
					}
				}
				moves[moveNum++] = move;
                break;
            }
            else
                moves[moveNum++] = move;
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
				for (j = 0; j < NUM_PIECES; j++) // put captured Piece type into move
				{
					if (move.movedPosition & boardState->boards[j + 2])
					{
						move.capturedPiece = j + 2;
						break;
					}
				}
				moves[moveNum++] = move;
                break;
            }
            else
                moves[moveNum++] = move;
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
				for (j = 0; j < NUM_PIECES; j++) // put captured Piece type into move
				{
					if (move.movedPosition & boardState->boards[j + 2])
					{
						move.capturedPiece = j + 2;
						break;
					}
				}
				moves[moveNum++] = move;
                break;
            }
            else
                moves[moveNum++] = move;
        }

        else
            break;
    }

    return moves;
}


Move* generateAllQueenMoves(BoardState* boardState,
                           enum BitboardType colorType)
{
  Move* moves = (Move*) malloc(sizeof(Move) * 200);
  int pieceNum = 0;
  
  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS] & boardState->boards[colorType];
  
  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;
    Move* setOfMoves = generateQueenMoves(boardState, isolatedPiece, colorType);
    
    memcpy((void*)&moves[pieceNum++ * 100], (void*)setOfMoves, sizeof(Move) * 100);
    
    // reset ls1b
    pieces &= pieces - 1;
  }
  
  
  int i;
  
  for(i = 0; i < 200; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_QUEEN_POSITIONS;
  }
  
  return moves;
}



Move* generateQueenMoves(BoardState* boardState,
                        Bitboard isolatedPiece,
                        enum BitboardType colorType)
{
  Move* moveSet1 = generateRookMoves(boardState, isolatedPiece, colorType);
  Move* moveSet2 = generateBishopMoves(boardState, isolatedPiece, colorType);
  Move* moves = (Move*) calloc(100, sizeof(Move) * 100);

  memcpy((void*)moves, (void*)moveSet1, sizeof(Move) * 50);
  memcpy((void*)&moves[50], (void*)moveSet2, sizeof(Move) * 50);

  return moves;
}


Move* generateAllKingMoves(BoardState* boardState,
                           enum BitboardType colorType)
{
  Move* moves = (Move*) calloc(50, sizeof(Move) * 50);
  int pieceNum = 0;
  
  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_KING_POSITIONS] & boardState->boards[colorType];
  
  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;
    Move* setOfMoves = generateKingMoves(boardState, isolatedPiece, colorType);
    
    memcpy((void*)&moves[pieceNum++ * 50], (void*)setOfMoves, sizeof(Move) * 50);
    
    // reset ls1b
    pieces &= pieces - 1;
  }
  
  
  int i;
  
  for(i = 0; i < 50; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_KING_POSITIONS;
  }
  
  return moves;
}



Move* generateKingMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType)
{
  Move* moves = (Move*) malloc(sizeof(Move) * 50);
  int moveNum = 0;
  
  int index;
  
  for(index = 0; index < 50; ++index)
  {
  }
  
  

  Move move = {0, 0, 0};


  typedef Move (*kingMove)(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           int offset);

  kingMove kingMoveGenerate[] = {generateSlideLeftMove,       generateSlideRightMove,
                                 generateSlideUpMove,         generateSlideDownMove,
                                 generateDiagonalUpRightMove, generateDiagonalDownRightMove,
                                 generateDiagonalUpLeftMove,  generateDiagonalDownLeftMove};


  int i;
  int j = 0; //for captured Piece type

  for(i = 0; i < sizeof(kingMoveGenerate) / sizeof(kingMoveGenerate[0]); ++i)
  {
    move = kingMoveGenerate[i](isolatedPiece, boardState, colorType, 1);

    if (move.movedPosition)
    {
      //capture move
		if (move.movedPosition & boardState->boards[!colorType])
		{
			for (j = 0; j < NUM_PIECES; j++) // put captured Piece type into move
			{
				if (move.movedPosition & boardState->boards[j + 2])
				{
					move.capturedPiece = j + 2;
					break;
				}
			}
      moves[moveNum++] = move;
		}

      else
        moves[moveNum++] = move;
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
  move.movedPosition  = move.initialPosition << (8 * offset);

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
  move.movedPosition  = move.initialPosition >> (8 * offset);

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





