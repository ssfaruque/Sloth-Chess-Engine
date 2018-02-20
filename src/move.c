
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


/*
moveGeneration possibleMoves[] = {generatePawnMoves,   generateRookMoves,
                                  generateKnightMoves, generateBishopMoves,
                                  generateQueenMoves,  generateKingMoves};

*/

typedef Move (*rookMove)(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           int offset);

            rookMove rookMoveGenerate[] = {generateSlideLeftMove, generateSlideRightMove,
                                 generateSlideUpMove, generateSlideDownMove};

typedef Move (*bishopMove)(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           int offset);

            bishopMove bishopMoveGenerate[] = {generateDiagonalDownLeftMove, generateDiagonalDownRightMove,
                                 generateDiagonalUpLeftMove, generateDiagonalUpRightMove};

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


int findCapturedPiece(BoardState* boardState, Bitboard movedPosition)
{
    for (int j = 0; j < NUM_PIECES; j++) //put captured Piece type into move
    {
        if (movedPosition & boardState->boards[j+2])
            return (j+2);
    }

    return 0;
}


/* Skeleton functions to fill out */

void generateAllSlidingMoves(BoardState* boardState,
                                  Bitboard isolatedPiece,
                                  int pieceType,
                                  enum BitboardType colorType,
                                  Moves* moves)
{
    Move move;
    int offset;

    switch (pieceType)
    {
        case BOARD_TYPE_ALL_PAWN_POSITIONS:
            if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
            {
                move = generateSlideUpMove(isolatedPiece, boardState, colorType, 1);

                //If valid and is not capture
                if (move.movedPosition &&
                    !(move.movedPosition & boardState->boards[!colorType]))
                    moves->moves[pieceType - 2][moves->numPawnMoves++] = move;

                if ((isolatedPiece & 0x000000000000ff00) != 0) //if in initial position
                {
                    move = generateSlideUpMove(isolatedPiece, boardState, colorType, 2);

                    //If valid and is not capture
                    if (move.movedPosition &&
                        !(move.movedPosition & boardState->boards[!colorType]))
                            moves->moves[pieceType - 2][moves->numPawnMoves++] = move;

                }
            }

            else if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
            {
                move = generateSlideDownMove(isolatedPiece, boardState, colorType, 1);

                //If valid and is not capture
                if (move.movedPosition &&
                    !(move.movedPosition & boardState->boards[!colorType]))
                    moves->moves[pieceType - 2][moves->numPawnMoves++] = move;

                if ((isolatedPiece & 0x000000000000ff00) != 0) //if in initial position
                {
                    move = generateSlideDownMove(isolatedPiece, boardState, colorType, 2);

                    //If valid and is not capture
                    if (move.movedPosition &&
                        !(move.movedPosition & boardState->boards[!colorType]))
                            moves->moves[pieceType - 2][moves->numPawnMoves++] = move;

                }
            }

        break; //pawns

        case BOARD_TYPE_ALL_ROOK_POSITIONS :
        case BOARD_TYPE_ALL_QUEEN_POSITIONS :

            for(int i = 0; i < sizeof(rookMoveGenerate) / sizeof(rookMoveGenerate[0]); ++i)
            {
                offset = 1;

                while(1)
                {
                    move = rookMoveGenerate[i](isolatedPiece, boardState, colorType, offset++);

                    if (move.movedPosition) // if valid
                    {
                        if (move.movedPosition & boardState->boards[!colorType]) //if capture
                        {
                            move.capturedPiece = findCapturedPiece(boardState, move.movedPosition);

                            if (pieceType == BOARD_TYPE_ALL_ROOK_POSITIONS)
                                moves->moves[pieceType - 2][moves->numRookMoves++] = move;
                            else
                                moves->moves[pieceType - 2][moves->numQueenMoves++] = move;
                            break;
                        }

                        else //quiet move
                        {
                            if (pieceType == BOARD_TYPE_ALL_ROOK_POSITIONS)
                                moves->moves[pieceType - 2][moves->numRookMoves++] = move;
                            else
                                moves->moves[pieceType - 2][moves->numQueenMoves++] = move;
                        }

                    } //if valid

                else
                    break;
                } //while 1
            } // looping through rookMoveGenerate
        break; //rooks and queens

        case BOARD_TYPE_ALL_KING_POSITIONS :

            for(int i = 0; i < sizeof(rookMoveGenerate) / sizeof(rookMoveGenerate[0]); ++i)
            {
                move = rookMoveGenerate[i](isolatedPiece, boardState, colorType, 1);

                if (move.movedPosition) // if valid
                {
                    if (move.movedPosition & boardState->boards[!colorType]) //if capture
                    {
                        move.capturedPiece = findCapturedPiece(boardState, move.movedPosition);
                        moves->moves[pieceType - 2][moves->numKingMoves++] = move;
                    }

                    else //quiet move
                        moves->moves[pieceType - 2][moves->numKingMoves++] = move;
                } //if valid

            } // looping through rookMoveGenerate
        break; //king

    } //switch Piecetype
    //add king queen
}



void generateAllDiagonalMoves(BoardState* boardState,
                                   Bitboard isolatedPiece,
                                   int pieceType,
                                   enum BitboardType colorType,
                                   Moves* moves)
{
    Move move;
    int offset;

    switch (pieceType)
    {
        case BOARD_TYPE_ALL_PAWN_POSITIONS:
            if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
            {
                  move = generateDiagonalUpLeftMove(isolatedPiece, boardState, colorType, 1);

                  if (move.movedPosition & boardState->boards[!colorType]) //if capture
                  {
                      move.capturedPiece = findCapturedPiece(boardState, move.movedPosition);
                      moves->moves[pieceType - 2][moves->numPawnMoves++] = move;
                  }

                  move = generateDiagonalUpRightMove(isolatedPiece, boardState, colorType, 1);

                  if (move.movedPosition & boardState->boards[!colorType]) //if capture
                  {
                      move.capturedPiece = findCapturedPiece(boardState, move.movedPosition);
                      moves->moves[pieceType - 2][moves->numPawnMoves++] = move;
                  }
            }

            else if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
            {
                  move = generateDiagonalDownLeftMove(isolatedPiece, boardState, colorType, 1);

                  if (move.movedPosition & boardState->boards[!colorType])
                  {
                      move.capturedPiece = findCapturedPiece(boardState, move.movedPosition);
                      moves->moves[pieceType - 2][moves->numPawnMoves++] = move;
                  }

                  move = generateDiagonalDownRightMove(isolatedPiece, boardState, colorType, 1);

                  if (move.movedPosition & boardState->boards[!colorType])
                  {
                      move.capturedPiece = findCapturedPiece(boardState, move.movedPosition);
                      moves->moves[pieceType - 2][moves->numPawnMoves++] = move;
                  }
            }

        break; //pawns

        case BOARD_TYPE_ALL_BISHOP_POSITIONS:
        case BOARD_TYPE_ALL_QUEEN_POSITIONS:
            for(int i = 0; i < sizeof(bishopMoveGenerate) / sizeof(bishopMoveGenerate[0]); ++i)
            {
                offset = 1;

                while(1)
                {
                    move = bishopMoveGenerate[i](isolatedPiece, boardState, colorType, offset++);

                    if (move.movedPosition) // if valid
                    {
                        if (move.movedPosition & boardState->boards[!colorType]) //if capture
                        {
                            move.capturedPiece = findCapturedPiece(boardState, move.movedPosition);

                            if (pieceType == BOARD_TYPE_ALL_BISHOP_POSITIONS)
                                moves->moves[pieceType - 2][moves->numBishopMoves++] = move;
                            else
                                moves->moves[pieceType - 2][moves->numQueenMoves++] = move;

                            break;
                        }

                        else //quiet move
                        {
                           if (pieceType == BOARD_TYPE_ALL_BISHOP_POSITIONS)
                                moves->moves[pieceType - 2][moves->numBishopMoves++] = move;
                            else
                                moves->moves[pieceType - 2][moves->numQueenMoves++] = move;
                        }

                    } //if valid

                else
                    break;
                } //while 1
            } // looping through rookMoveGenerate
        break; //rooks and queens

        case BOARD_TYPE_ALL_KING_POSITIONS :

            for(int i = 0; i < sizeof(bishopMoveGenerate) / sizeof(bishopMoveGenerate[0]); ++i)
            {
                move = bishopMoveGenerate[i](isolatedPiece, boardState, colorType, 1);

                if (move.movedPosition) // if valid
                {
                    if (move.movedPosition & boardState->boards[!colorType]) //if capture
                    {
                        move.capturedPiece = findCapturedPiece(boardState, move.movedPosition);
                        moves->moves[pieceType - 2][moves->numKingMoves++] = move;
                    }

                    else //quiet move
                        moves->moves[pieceType - 2][moves->numKingMoves++] = move;
                } //if valid

            } // looping through rookMoveGenerate
        break; //king

    } //switch Piecetype

}






















/*
int negaMax(BoardState* boardState,
            enum BitboardType colorType,
            int depth)
{
  if (depth == 0)
    return eval(boardState);

  int max = -111111111;

  Moves moves;
  generateAllMoves(boardState, colorType, &moves);
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
*/


/*
Move generateMove(BoardState* boardState,
                  enum BitboardType colorType,
                  int recurseDepth)
{
  Move move = {0, 0, 0, 0, 0};

  int numFirstMoves = 950;
  int i;

  int score = 0;
  int maxScore = 0;

  Moves firstMoves;

  generateAllMoves(boardState, colorType, &firstMoves);

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
        move.boardEval = score;
      }

    }

  }

  free(firstMoves);

  return move;
}
*/



void generateAllMoves(BoardState* boardState,
                      enum BitboardType colorType,
                      Moves* moves)
{
  generateAllPawnMoves(boardState, colorType, moves);
  generateAllRookMoves(boardState, colorType, moves);
  generateAllKnightMoves(boardState, colorType, moves);
  generateAllBishopMoves(boardState, colorType, moves);
  //generateAllQueenMoves(boardState, colorType, moves);
  generateAllKingMoves(boardState, colorType, moves);
}




void generateAllPawnMoves(BoardState* boardState,
                          enum BitboardType colorType,
                          Moves* moves)
{
  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_PAWN_POSITIONS] & boardState->boards[colorType];

  int i;

  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;

    generatePawnMoves(boardState, isolatedPiece, colorType, moves);

    // reset ls1b
    pieces &= pieces - 1;
  }

  /*
  for(i = 0; i < 400; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_PAWN_POSITIONS;
  }*/

}



void generatePawnMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType,
                       Moves* moves)
{
  int i = 0; //For captured Piece type
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
      moves->moves[BOARD_TYPE_ALL_PAWN_POSITIONS - 2][moves->numPawnMoves++] = move;


    /*Generate Slide Up 2 when unmoved*/
    if((isolatedPiece & 0x000000000000ff00) != 0) // if in initial position
    {
      move = generateSlideUpMove(isolatedPiece, boardState, colorType, 2);
      // If Valid and is not Capture, added to moves.
      if(move.movedPosition &&
          !(move.movedPosition & boardState->boards[!colorType]))
        moves->moves[BOARD_TYPE_ALL_PAWN_POSITIONS - 2][moves->numPawnMoves++] = move;

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
        moves->moves[BOARD_TYPE_ALL_PAWN_POSITIONS - 2][moves->numPawnMoves++] = move;

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
			  moves->moves[BOARD_TYPE_ALL_PAWN_POSITIONS - 2][moves->numPawnMoves++] = move;

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
      moves->moves[BOARD_TYPE_ALL_PAWN_POSITIONS - 2][moves->numPawnMoves++] = move;

    /*Generate Slide Up 2 when unmoved*/
    if ((isolatedPiece & 0x00ff000000000000) != 0) //if in initial position
    {
      move = generateSlideDownMove(isolatedPiece, boardState, colorType, 2);

      // If Valid and is not Capture, added to moves
      if (move.movedPosition &&
          !(move.movedPosition & boardState->boards[!colorType]))
        moves->moves[BOARD_TYPE_ALL_PAWN_POSITIONS - 2][moves->numPawnMoves++] = move;
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
			  moves->moves[BOARD_TYPE_ALL_PAWN_POSITIONS - 2][moves->numPawnMoves++] = move;

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
			  moves->moves[BOARD_TYPE_ALL_PAWN_POSITIONS - 2][moves->numPawnMoves++] = move;

		  }



  }

}



void generateAllRookMoves(BoardState* boardState,
                           enum BitboardType colorType,
                          Moves* moves)
{
  int pieceNum = 0;

  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_ROOK_POSITIONS] & boardState->boards[colorType];

  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;

    generateRookMoves(boardState, isolatedPiece, colorType, moves);


    // reset ls1b
    pieces &= pieces - 1;
  }


  int i;

  /*
  for(i = 0; i < 100; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_ROOK_POSITIONS;
  }
  */
}




void generateRookMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType,
                       Moves* moves)
{
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
		  moves->moves[BOARD_TYPE_ALL_ROOK_POSITIONS - 2][moves->numRookMoves++] = move;
        break;
      }
      else//quiet
        moves->moves[BOARD_TYPE_ALL_ROOK_POSITIONS - 2][moves->numRookMoves++] = move;
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
		  moves->moves[BOARD_TYPE_ALL_ROOK_POSITIONS - 2][moves->numRookMoves++] = move;
        break;
      }
      else//quiet
        moves->moves[BOARD_TYPE_ALL_ROOK_POSITIONS - 2][moves->numRookMoves++] = move;
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
		  moves->moves[BOARD_TYPE_ALL_ROOK_POSITIONS - 2][moves->numRookMoves++] = move;
        break;
      }
      else//quiet
        moves->moves[BOARD_TYPE_ALL_ROOK_POSITIONS - 2][moves->numRookMoves++] = move;
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
		  moves->moves[BOARD_TYPE_ALL_ROOK_POSITIONS - 2][moves->numRookMoves++] = move;
        break;
      }
      else//quiet
        moves->moves[BOARD_TYPE_ALL_ROOK_POSITIONS - 2][moves->numRookMoves++] = move;
    }
    else
      break;
  }

}


void generateAllKnightMoves(BoardState* boardState,
                           enum BitboardType colorType,
                            Moves* moves)
{
  int pieceNum = 0;

  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_KNIGHT_POSITIONS] & boardState->boards[colorType];

  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;

    generateKnightMoves(boardState, isolatedPiece, colorType, moves);


    // reset ls1b
    pieces &= pieces - 1;
  }

  int i;

  /*
  for(i = 0; i < 100; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_KNIGHT_POSITIONS;
  }
  */

}




void generateKnightMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType,
                         Moves* moves)
{
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
			moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
		}

        else
            moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
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
			moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
		}
        else
            moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
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
			moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
		}

        else
            moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
    }

    move = generateUpRightRight(isolatedPiece, boardState, colorType);

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
      moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
    }

    else
      moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
  }

  move = generateDownDownLeft(isolatedPiece, boardState, colorType);


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
			moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
		}

        else
            moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
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
			moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
		}

        else
            moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
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
			moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
		}
        else
            moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
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
			moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
		}

        else
            moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
    }

}



void generateAllBishopMoves(BoardState* boardState,
                           enum BitboardType colorType,
                            Moves* moves)
{
  int pieceNum = 0;

  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_BISHOP_POSITIONS] & boardState->boards[colorType];

  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;

    generateBishopMoves(boardState, isolatedPiece, colorType, moves);


    // reset ls1b
    pieces &= pieces - 1;
  }


  int i;

  /*
  for(i = 0; i < 100; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_BISHOP_POSITIONS;
  }
  */
}




void generateBishopMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType,
                         Moves* moves)
{
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
				moves->moves[BOARD_TYPE_ALL_BISHOP_POSITIONS - 2][moves->numBishopMoves++] = move;
                break;
            }

            else
                moves->moves[BOARD_TYPE_ALL_BISHOP_POSITIONS - 2][moves->numBishopMoves++] = move;
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
				moves->moves[BOARD_TYPE_ALL_BISHOP_POSITIONS - 2][moves->numBishopMoves++] = move;
                break;
            }
            else
                moves->moves[BOARD_TYPE_ALL_BISHOP_POSITIONS - 2][moves->numBishopMoves++] = move;
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
				moves->moves[BOARD_TYPE_ALL_BISHOP_POSITIONS - 2][moves->numBishopMoves++] = move;
                break;
            }
            else
               moves->moves[BOARD_TYPE_ALL_BISHOP_POSITIONS - 2][moves->numBishopMoves++] = move;
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
				moves->moves[BOARD_TYPE_ALL_BISHOP_POSITIONS - 2][moves->numBishopMoves++] = move;
                break;
            }
            else
                moves->moves[BOARD_TYPE_ALL_BISHOP_POSITIONS - 2][moves->numBishopMoves++] = move;
        }

        else
            break;
    }

}

/*
void generateAllQueenMoves(BoardState* boardState,
                           enum BitboardType colorType,
                           Moves* moves)
{
  int pieceNum = 0;

  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS] & boardState->boards[colorType];

  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;

    generateQueenMoves(boardState, isolatedPiece, colorType, moves);


    // reset ls1b
    pieces &= pieces - 1;
  }


  int i;


  for(i = 0; i < 200; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_QUEEN_POSITIONS;
  }

}




void generateQueenMoves(BoardState* boardState,
                        Bitboard isolatedPiece,
                        enum BitboardType colorType,
                        Moves* moves)
{
  Move* moveSet1 = generateRookMoves(boardState, isolatedPiece, colorType);
  Move* moveSet2 = generateBishopMoves(boardState, isolatedPiece, colorType);

}
*/



void generateAllKingMoves(BoardState* boardState,
                           enum BitboardType colorType,
                          Moves* moves)
{
  int pieceNum = 0;

  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_KING_POSITIONS] & boardState->boards[colorType];

  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;

    generateKingMoves(boardState, isolatedPiece, colorType, moves);


    // reset ls1b
    pieces &= pieces - 1;
  }


  int i;
  /*
  for(i = 0; i < 50; ++i)
  {
    moves[i].pieceType = BOARD_TYPE_ALL_KING_POSITIONS;
  }
  */
}



void generateKingMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType,
                       Moves* moves)
{
  int moveNum = 0;

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
      moves->moves[BOARD_TYPE_ALL_KING_POSITIONS - 2][moves->numKingMoves++] = move;
		}

      else
        moves->moves[BOARD_TYPE_ALL_KING_POSITIONS - 2][moves->numKingMoves++] = move;
    }
  }

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





