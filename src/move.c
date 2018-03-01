
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


typedef Move (*knightMove)(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType
                           );

            knightMove knightMoveGenerate[] = {generateUpLeftLeft, generateUpUpLeft, generateUpUpRight, generateUpRightRight,
                                 generateDownRightRight, generateDownDownRight, generateDownDownLeft, generateDownLeftLeft};

int WHITE_LEFT_CASTLE = 1;
int WHITE_RIGHT_CASTLE = 1;
int BLACK_LEFT_CASTLE = 1;
int BLACK_RIGHT_CASTLE = 1;


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
                      enum CastlingType castling,
                      int capturedPiece,
                      int undo)
{
	if (undo && !capturedPiece) //undo quiet
	{  /*Pawn Promotion*/
		if (pieceType == BOARD_TYPE_ALL_PAWN_POSITIONS && ((colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS &&
			(movedPiece & 0xff00000000000000)) || (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS &&
			(movedPiece & 0x00000000000000ff))))// if promotion
		{
			boardState->boards[colorType] = ((boardState->boards[colorType] ^ movedPiece) | initialPiece);
			boardState->boards[pieceType] = (boardState->boards[pieceType] | initialPiece);
			boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS] =
				(boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS] ^ movedPiece);

		}
		else // undo normal quiet
		{
			boardState->boards[colorType] = ((boardState->boards[colorType] ^ movedPiece) | initialPiece);
			boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ movedPiece) | initialPiece);
		}
	}

      else if(!capturedPiece) // quiet
      {
		  /*Pawn Promotion*/
		  if (pieceType == BOARD_TYPE_ALL_PAWN_POSITIONS && ((colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS &&
			  (movedPiece & 0xff00000000000000)) || (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS &&
			  (movedPiece & 0x00000000000000ff))))// if promotion
		  {
			  boardState->boards[colorType] = ((boardState->boards[colorType] ^ initialPiece) | movedPiece);
			  boardState->boards[pieceType] = boardState->boards[pieceType] ^ initialPiece;
			  boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS] =
				  ((boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS]) | movedPiece);

		  }


		  else  // not promotion
		  {
			  boardState->boards[colorType] = ((boardState->boards[colorType] ^ initialPiece) | movedPiece);
			  boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ initialPiece) | movedPiece);
		  }
	  }

      else if (capturedPiece && undo) // undo capture
      {

		  // Promote capture undo
		  if (pieceType == BOARD_TYPE_ALL_PAWN_POSITIONS && ((colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS &&
			  (movedPiece & 0xff00000000000000)) || (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS &&
			  (movedPiece & 0x00000000000000ff))))
		  {
			  boardState->boards[colorType] = ((boardState->boards[colorType] ^ movedPiece) | initialPiece);
			  boardState->boards[!colorType] = ((boardState->boards[!colorType]) | movedPiece);
			  boardState->boards[pieceType] = boardState->boards[pieceType] | initialPiece;
			  boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS] =
				  (boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS] ^ movedPiece); // Delete queen
			  boardState->boards[capturedPiece] = (boardState->boards[capturedPiece]) | movedPiece;//Add captured back

		  } //Promote capture undo
		  else // normal capture undo
		  {
			  boardState->boards[colorType] = ((boardState->boards[colorType] ^ movedPiece) | initialPiece);
			  boardState->boards[!colorType] = ((boardState->boards[!colorType]) | movedPiece);


			  if (capturedPiece == pieceType)
			  {
				  boardState->boards[pieceType] = (boardState->boards[pieceType] | initialPiece);
			  }

			  else
			  {
				  boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ movedPiece) | initialPiece);
				  boardState->boards[capturedPiece] = (boardState->boards[capturedPiece]) | movedPiece;
			  }
		  }
      } // undo capture

      else if(capturedPiece) //capture
      {
		  /*Pawn Promotion*/
		  if (pieceType == BOARD_TYPE_ALL_PAWN_POSITIONS && ((colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS &&
			  (movedPiece & 0xff00000000000000)) || (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS &&
			  (movedPiece & 0x00000000000000ff))))// if promotion
		  {
			  boardState->boards[capturedPiece] = (boardState->boards[capturedPiece]) ^ movedPiece;
			  boardState->boards[!colorType] = ((boardState->boards[!colorType]) ^ movedPiece);
			  boardState->boards[colorType] = ((boardState->boards[colorType] ^ initialPiece) | movedPiece);
			  boardState->boards[pieceType] = (boardState->boards[pieceType] ^ initialPiece);
			  boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS] =
				  (boardState->boards[BOARD_TYPE_ALL_QUEEN_POSITIONS] | movedPiece);
		  }
		  else // normal capture
		  {
			  boardState->boards[capturedPiece] = (boardState->boards[capturedPiece]) ^ movedPiece;
			  boardState->boards[!colorType] = ((boardState->boards[!colorType]) ^ movedPiece);
			  boardState->boards[colorType] = ((boardState->boards[colorType] ^ initialPiece) | movedPiece);
			  boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ initialPiece) | movedPiece);
		  }
      } //capture


      Bitboard king_initial, king_final;

      if (castling)         //castling updates for king
      {
            if (castling == QUEENS_SIDE && undo)
            {

                if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
                {
                    BLACK_LEFT_CASTLE = 1;
                    king_initial = 0x2000000000000000;
                    king_final = 0x0800000000000000;
                }
                else
                {
                    WHITE_LEFT_CASTLE = 1;
                    king_initial = 0x0000000000000020;
                    king_final = 0x0000000000000008;
                }
            }

            else if (castling == KINGS_SIDE && undo)
            {
                if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
                {
                    BLACK_RIGHT_CASTLE = 1;
                    king_initial = 0x0200000000000000;
                    king_final = 0x0800000000000000;
                }
                else
                {
                    WHITE_RIGHT_CASTLE = 1;
                    king_initial = 0x0000000000000002;
                    king_final = 0x0000000000000008;
                }

            }

            else if (castling == QUEENS_SIDE)
            {
                if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
                {
                    BLACK_LEFT_CASTLE = 0;
                    king_initial = 0x0800000000000000;
                    king_final = 0x2000000000000000;
                }
                else
                {
                    WHITE_LEFT_CASTLE = 0;
                    king_initial = 0x0000000000000008;
                    king_final = 0x0000000000000020;
                }
            }

            else if (castling == KINGS_SIDE)
            {
                if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
                {
                    BLACK_RIGHT_CASTLE = 0;
                    king_initial = 0x0800000000000000;
                    king_final = 0x0200000000000000;
                }
                else
                {
                    WHITE_RIGHT_CASTLE = 0;
                    king_initial = 0x0000000000000008;
                    king_final = 0x0000000000000002;
                }
            }

            boardState->boards[colorType] = ((boardState->boards[colorType] ^ king_initial) | king_final);
            boardState->boards[BOARD_TYPE_ALL_KING_POSITIONS] = ((boardState->boards[BOARD_TYPE_ALL_KING_POSITIONS] ^ king_initial) | king_final);

      } //if castling


  //Keeping track of castling legality
    if (pieceType == BOARD_TYPE_ALL_KING_POSITIONS)
    {
        if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS && initialPiece == 0x0000000000000008)
        {
            if (undo)
            {
                WHITE_LEFT_CASTLE = 1;
                WHITE_RIGHT_CASTLE = 1;
            }

            else
            {
                WHITE_LEFT_CASTLE = 0;
                WHITE_RIGHT_CASTLE = 0;
            }
        }

        else if (initialPiece == 0x0800000000000000) //black
        {
            if (undo)
            {
                BLACK_LEFT_CASTLE = 1;
                BLACK_RIGHT_CASTLE = 1;
            }
            else
            {
                BLACK_LEFT_CASTLE = 0;
                BLACK_RIGHT_CASTLE = 0;
            }
        }
    }


    if (pieceType == BOARD_TYPE_ALL_ROOK_POSITIONS)
    {
        if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
        {
            if (initialPiece == 0x8000000000000000) //initial position of black rook
                BLACK_LEFT_CASTLE = 0;
            if (initialPiece == 0x0100000000000000)
                BLACK_RIGHT_CASTLE = 0;
        }

        else
        {
            if (initialPiece == 0x0000000000000001) //initial position of white
                WHITE_RIGHT_CASTLE = 0;
            if (initialPiece == 0x0000000000000080)
                WHITE_LEFT_CASTLE = 0;
        }
    } // if pieceType = rook

  return 0;
} //update board state



/******************************** Generate moves by pieces ********************************/


int findCapturedPiece(BoardState* boardState, Bitboard movedPosition, enum BitboardType colorType)
{
    int j = 0;
    for (j = 0; j < NUM_PIECES; j++) //put captured Piece type into move
    {
        if (movedPosition & (boardState->boards[!colorType] & boardState->boards[j+2]))
            return (j+2);
    }

    return 0;
}

int castlingLeftCheck(enum BitboardType colorType, Bitboard movedPosition)
{

    if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
        if (movedPosition == 0x0000000000000010)
            return 1;


    if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
        if (movedPosition == 0x1000000000000000)
            return 1;

    return 0;
}


int castlingRightCheck(enum BitboardType colorType, Bitboard movedPosition)
{
    if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
    {
        if (movedPosition == 0x0000000000000004)
            return 1;
    }

    else
    {
        if (movedPosition == 0x0400000000000000)
            return 1;
    }

  return 0;

}

int isRookinCheck(BoardState* boardState, enum BitboardType colorType, enum CastlingType castling)
{
    Bitboard rooktoCheck;

    if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
    {
        if (castling == QUEENS_SIDE)
            rooktoCheck = 0x1000000000000000;
        else    //KINGS_SIDE
            rooktoCheck = 0x0400000000000000;
    }

    else if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
    {
        if (castling == QUEENS_SIDE)
            rooktoCheck = 0x0000000000000010;
        else    //KINGS_SIDE
            rooktoCheck = 0x0000000000000004;
    }


    Moves moves;
    moves.numMoves[0] = moves.numMoves[1] = moves.numMoves[2] =
    moves.numMoves[3] = moves.numMoves[4] = moves.numMoves[5] = 0;

    generateAllMoves(boardState, !colorType, &moves);
    int i  = 0; int j = 0;
    for ( i = 0; i < NUM_PIECES; i ++)
        for ( j = 0; j < moves.numMoves[i]; j ++)
            if (moves.moves[i][j].capturedPiece == BOARD_TYPE_ALL_ROOK_POSITIONS
                && moves.moves[i][j].movedPosition == rooktoCheck) //if the captured piece is the opponent's king
                    return 1;

  return 0;

}

/* Skeleton functions to fill out */

void generateAllSlidingMoves(BoardState* boardState,
                                  Bitboard isolatedPiece,
                                  int pieceType,
                                  enum BitboardType colorType,
                                  Moves* moves)
{
  Move move = {0, 0, 0, 0, 0, 0};
    int offset;
    int i = 0;
    switch (pieceType)
    {
        case BOARD_TYPE_ALL_PAWN_POSITIONS:
            if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
            {
                move = generateSlideUpMove(isolatedPiece, boardState, colorType, 1);
                move.pieceType = pieceType;

                //If valid and is not capture
                if (move.movedPosition &&
                    !(move.movedPosition & boardState->boards[!colorType]))
                    moves->moves[pieceType - 2][moves->numPawnMoves++] = move;

                if (((isolatedPiece & 0x000000000000ff00) != 0 ) &&
					!((isolatedPiece << 8) & boardState->boards[!colorType] ) &&
					!((isolatedPiece << 8) & boardState->boards[colorType]) ) //if in initial position, move up by 2
                {
                    move = generateSlideUpMove(isolatedPiece, boardState, colorType, 2);
                    move.pieceType = pieceType;

                    //If valid and is not capture
                    if (move.movedPosition &&
                        !(move.movedPosition & boardState->boards[!colorType]))
                            moves->moves[pieceType - 2][moves->numPawnMoves++] = move;

                } //pawn in initial position
            } //if white

            else if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
            {
                move = generateSlideDownMove(isolatedPiece, boardState, colorType, 1);
                move.pieceType = pieceType;

                //If valid and is not capture
                if (move.movedPosition &&
                    !(move.movedPosition & boardState->boards[!colorType]))
                    moves->moves[pieceType - 2][moves->numPawnMoves++] = move;

                if ((isolatedPiece & 0x00ff000000000000) != 0	&&
					!((isolatedPiece >> 8) & boardState->boards[!colorType]) &&
					!((isolatedPiece >> 8) & boardState->boards[colorType])) //if in initial position
                {
                    move = generateSlideDownMove(isolatedPiece, boardState, colorType, 2);
                    move.pieceType = pieceType;

                    //If valid and is not capture
                    if (move.movedPosition &&
                        !(move.movedPosition & boardState->boards[!colorType]))
                            moves->moves[pieceType - 2][moves->numPawnMoves++] = move;

                } //initial pawn position
            } //color black

        break; //pawns

        case BOARD_TYPE_ALL_ROOK_POSITIONS :
        case BOARD_TYPE_ALL_QUEEN_POSITIONS :

            for( i = 0; i < sizeof(rookMoveGenerate) / sizeof(rookMoveGenerate[0]); ++i)
            {
                offset = 1;

                while(1)
                {
                    move = rookMoveGenerate[i](isolatedPiece, boardState, colorType, offset++);
                    move.pieceType = pieceType;

                    if (move.movedPosition) // if valid
                    {
                        if (move.movedPosition & boardState->boards[!colorType]) //if capture
                        {
                            move.capturedPiece = findCapturedPiece(boardState, move.movedPosition, colorType);

                            if (pieceType == BOARD_TYPE_ALL_ROOK_POSITIONS)
                                moves->moves[pieceType - 2][moves->numRookMoves++] = move;
                            else
                                moves->moves[pieceType - 2][moves->numQueenMoves++] = move;
                            break;
                        } //if capture

                        else //quiet move
                        {
                            if (pieceType == BOARD_TYPE_ALL_ROOK_POSITIONS)
                            {
                                moves->moves[pieceType - 2][moves->numRookMoves++] = move;

                           /*     if (WHITE_LEFT_CASTLE)
                                {
                                    if (i == 1 && move.initialPosition == 0x0000000000000080 && move.movedPosition == 0x00000000000000010) //check for rook slide right and legal castling
                                    {
                                        move.castling = QUEENS_SIDE;
                                        moves->moves[pieceType - 2][moves->numRookMoves++] = move;
                                    }
                                } //castle white left

                                if (BLACK_LEFT_CASTLE)
                                {
                                    if (i == 1 && move.initialPosition == 0x8000000000000000 && move.movedPosition == 0x1000000000000000) //check for rook slide right and legal castling
                                    {
                                        move.castling = QUEENS_SIDE;
                                        moves->moves[pieceType - 2][moves->numRookMoves++] = move;
                                    }
                                } //castle black queens side

                                if (WHITE_RIGHT_CASTLE)
                                {
                                  if (i == 0 && move.initialPosition == 0x0000000000000001 && move.movedPosition == 0x00000000000000004) //check for rook slide left and legal castling
                                    {
                                        move.castling = KINGS_SIDE;
                                        moves->moves[pieceType - 2][moves->numRookMoves++] = move;
                                    }
                                } //castle white kings side


                                if (BLACK_RIGHT_CASTLE)
                                {
                                    if (i == 0 && move.initialPosition == 0x0100000000000000 && move.movedPosition == 0x04000000000000000) //check for rook slide left and legal castling
                                    {
                                        move.castling = KINGS_SIDE;
                                        moves->moves[pieceType - 2][moves->numRookMoves++] = move;
                                    }
                                } //castle black kings side */
                            } // Rook

                            else //QUEEN
                                moves->moves[pieceType - 2][moves->numQueenMoves++] = move;
                        }

                    } //if valid

                else
                    break;
                } //while 1
            } // looping through rookMoveGenerate
        break; //rooks and queens

        case BOARD_TYPE_ALL_KING_POSITIONS :

            for( i = 0; i < sizeof(rookMoveGenerate) / sizeof(rookMoveGenerate[0]); ++i)
            {
                move = rookMoveGenerate[i](isolatedPiece, boardState, colorType, 1);
                move.pieceType = pieceType;

                if (move.movedPosition) // if valid
                {
                    if (move.movedPosition & boardState->boards[!colorType]) //if capture
                        move.capturedPiece = findCapturedPiece(boardState, move.movedPosition, colorType);

                    moves->moves[pieceType - 2][moves->numKingMoves++] = move;

                } //if valid

            } // looping through rookMoveGenerate
        break; //king

    } //switch Piecetype

} //generate all sliding moves



void generateAllDiagonalMoves(BoardState* boardState,
                                   Bitboard isolatedPiece,
                                   int pieceType,
                                   enum BitboardType colorType,
                                   Moves* moves)
{
    Move move = {0, 0, 0, 0, 0, 0};
    int offset;
    int i = 0;
    switch (pieceType)
    {
        case BOARD_TYPE_ALL_PAWN_POSITIONS:
            if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
            {
                  move = generateDiagonalUpLeftMove(isolatedPiece, boardState, colorType, 1);
                  move.pieceType = pieceType;

                  if (move.movedPosition & boardState->boards[!colorType]) //if capture
                  {
                      move.capturedPiece = findCapturedPiece(boardState, move.movedPosition, colorType);
                      moves->moves[pieceType - 2][moves->numPawnMoves++] = move;
                  }

                  move = generateDiagonalUpRightMove(isolatedPiece, boardState, colorType, 1);
                  move.pieceType = pieceType;

                  if (move.movedPosition & boardState->boards[!colorType]) //if capture
                  {
                      move.capturedPiece = findCapturedPiece(boardState, move.movedPosition, colorType);
                      moves->moves[pieceType - 2][moves->numPawnMoves++] = move;
                  }
            } // white color

            else if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
            {
                  move = generateDiagonalDownLeftMove(isolatedPiece, boardState, colorType, 1);
                  move.pieceType = pieceType;

                  if (move.movedPosition & boardState->boards[!colorType])
                  {
                      move.capturedPiece = findCapturedPiece(boardState, move.movedPosition, colorType);
                      moves->moves[pieceType - 2][moves->numPawnMoves++] = move;
                  }

                  move = generateDiagonalDownRightMove(isolatedPiece, boardState, colorType, 1);
                  move.pieceType = pieceType;

                  if (move.movedPosition & boardState->boards[!colorType])
                  {
                      move.capturedPiece = findCapturedPiece(boardState, move.movedPosition, colorType);
                      moves->moves[pieceType - 2][moves->numPawnMoves++] = move;
                  }
            } // black color

        break; //pawns

        case BOARD_TYPE_ALL_BISHOP_POSITIONS:
        case BOARD_TYPE_ALL_QUEEN_POSITIONS:
            for(i = 0; i < sizeof(bishopMoveGenerate) / sizeof(bishopMoveGenerate[0]); ++i)
            {
                offset = 1;

                while(1)
                {
                    move = bishopMoveGenerate[i](isolatedPiece, boardState, colorType, offset++);
                    move.pieceType = pieceType;

                    if (move.movedPosition) // if valid
                    {
                        if (move.movedPosition & boardState->boards[!colorType]) //if capture
                        {
                            move.capturedPiece = findCapturedPiece(boardState, move.movedPosition, colorType);

                            if (pieceType == BOARD_TYPE_ALL_BISHOP_POSITIONS)
                                moves->moves[pieceType - 2][moves->numBishopMoves++] = move;
                            else
                                moves->moves[pieceType - 2][moves->numQueenMoves++] = move;

                            break;
                        } // if capture

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

            for(i = 0; i < sizeof(bishopMoveGenerate) / sizeof(bishopMoveGenerate[0]); ++i)
            {
                move = bishopMoveGenerate[i](isolatedPiece, boardState, colorType, 1);
                move.pieceType = pieceType;

                if (move.movedPosition) // if valid
                {
                    if (move.movedPosition & boardState->boards[!colorType]) //if capture
                        move.capturedPiece = findCapturedPiece(boardState, move.movedPosition, colorType);

                    moves->moves[pieceType - 2][moves->numKingMoves++] = move;
                } //if valid

            } // looping through rookMoveGenerate
        break; //king

    } //switch Piecetype

} //generate all diagonal moves




int maxi(BoardState* boardState,
            enum BitboardType colorType, int depth)
{
    if (depth == 0) return eval(boardState);

    int max = -11111111;


  Moves moves;
  moves.numMoves[0] = moves.numMoves[1] = moves.numMoves[2] =
  moves.numMoves[3] = moves.numMoves[4] = moves.numMoves[5] = 0;
  generateAllMoves(boardState, colorType, &moves);

  int score = 0;
  int i;
  int j;


    for(i = 0; i < NUM_PIECES; ++i)
    {
        for(j = 0; j < moves.numMoves[i]; ++j)
        {

            if(moves.moves[i][j].initialPosition) //if valid
            {
                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 0);

                if (isKingInCheck(boardState, colorType)) // the player's move leaves the player's king in check
                {
                    //don't recurse down, undo and go to next move
                    updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 1);
                    continue;

                }

                score = mini(boardState, !colorType, depth - 1);

                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 1);

                if (score > max)
                    max = score;

            }
        }
    }

    return max;
}



int mini(BoardState* boardState,
            enum BitboardType colorType, int depth)
{
    if (depth == 0) return eval(boardState);

    int min = 11111111;

  Moves moves;
  moves.numMoves[0] = moves.numMoves[1] = moves.numMoves[2] =
  moves.numMoves[3] = moves.numMoves[4] = moves.numMoves[5] = 0;
  generateAllMoves(boardState, colorType, &moves);

  int score = 0;
  int i;
  int j;


    for(i = 0; i < NUM_PIECES; ++i)
    {
        for(j = 0; j < moves.numMoves[i]; ++j)
        {

            if(moves.moves[i][j].initialPosition) //if valid
            {
                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 0);

                if (isKingInCheck(boardState, colorType)) // the player's move leaves the player's king in check
                {
                    //don't recurse down, undo and go to next move
                    updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 1);
                    continue;

                }

                score = maxi(boardState, !colorType, depth - 1);

                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 1);

                if (score < min)
                    min = score;

            }
        }
    }

    return min;
}



int alphaBetaMax(BoardState* boardState, int alpha, int beta, enum BitboardType colorType, int depthleft)
{
    if (depthleft == 0) return eval(boardState);

    //int max = -11111111;


  Moves moves;
  moves.numMoves[0] = moves.numMoves[1] = moves.numMoves[2] =
  moves.numMoves[3] = moves.numMoves[4] = moves.numMoves[5] = 0;
  generateAllMoves(boardState, colorType, &moves);

  int score = 0;
  int i;
  int j;
    int moveEval;

    for(i = 0; i < NUM_PIECES; ++i)
    {
        for(j = 0; j < moves.numMoves[i]; ++j)
        {

            if(moves.moves[i][j].initialPosition) //if valid
            {
                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 0);
                moveEval = eval(boardState);

                if (isKingInCheck(boardState, colorType)) // the player's move leaves the player's king in check
                {
                    //don't recurse down, undo and go to next move
                    updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 1);
                    continue;

                }

                if (moves.moves[i][j].castling)
                {
                    if (isRookinCheck(boardState, colorType, moves.moves[i][j].castling))
                    {
                        // dont recurse down
                        updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 1);
                        continue;
                    }
                }

                //score = moveEval + alphaBetaMin(boardState, alpha, beta, !colorType, depthleft - 1);
                score = alphaBetaMin(boardState, alpha, beta, !colorType, depthleft - 1);

                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 1);

                if (score >= beta)
                    return beta;

                if (score > alpha)
                    alpha = score;
            }
        }
    }

    return alpha;
}

int alphaBetaMin(BoardState* boardState, int alpha, int beta, enum BitboardType colorType, int depthleft)
{
    if (depthleft == 0) return eval(boardState);

    //int max = -11111111;


  Moves moves;
  moves.numMoves[0] = moves.numMoves[1] = moves.numMoves[2] =
  moves.numMoves[3] = moves.numMoves[4] = moves.numMoves[5] = 0;
  generateAllMoves(boardState, colorType, &moves);

  int score = 0;
  int i;
  int j;
    int moveEval = 0;

    for(i = 0; i < NUM_PIECES; ++i)
    {
        for(j = 0; j < moves.numMoves[i]; ++j)
        {

            if(moves.moves[i][j].initialPosition) //if valid
            {
                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 0);
                //moveEval = eval(boardState);

                if (isKingInCheck(boardState, colorType)) // the player's move leaves the player's king in check
                {
                    //don't recurse down, undo and go to next move
                    updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 1);
                    continue;

                }

                if (moves.moves[i][j].castling)
                {
                    if (isRookinCheck(boardState, colorType, moves.moves[i][j].castling))
                    {
                        // dont recurse down
                        updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 1);
                        continue;
                    }
                }

                //score = moveEval + alphaBetaMax(boardState, alpha, beta, !colorType, depthleft - 1);
                score = alphaBetaMax(boardState, alpha, beta, !colorType, depthleft - 1);
                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, moves.moves[i][j].castling, moves.moves[i][j].capturedPiece, 1);

                if (score <= alpha)
                    return alpha;

                if (score < beta)
                    beta = score;
            }
        }
    }

    return beta;
}

Move generateMove(BoardState* boardState,
                  enum BitboardType colorType,
                  int recurseDepth)
{
  Move move = {0, 0, 0, 0, 0, 0};

  int i;
  int j;

  int firstMovesEval = 0;
  int maxScore = -111111111;
  int minScore = 111111111;
  int score = 0;

  Moves firstMoves;
  firstMoves.numMoves[0] = firstMoves.numMoves[1] = firstMoves.numMoves[2] =
  firstMoves.numMoves[3] = firstMoves.numMoves[4] = firstMoves.numMoves[5] = 0;
  generateAllMoves(boardState, colorType, &firstMoves);

  for(i = 0; i < NUM_PIECES; ++i)
  {

    for(j = 0; j < firstMoves.numMoves[i]; ++j)
    {

      if(firstMoves.moves[i][j].initialPosition)
      {
        // do the move
        updateBoardState(boardState, firstMoves.moves[i][j].initialPosition, firstMoves.moves[i][j].movedPosition, colorType, firstMoves.moves[i][j].pieceType, firstMoves.moves[i][j].castling, firstMoves.moves[i][j].capturedPiece, 0);
        //firstMovesEval = eval(boardState);

        if (isKingInCheck(boardState, colorType)) // the player's move leaves the player's king in check
        {
            //don't recurse down, undo and go to next move
                updateBoardState(boardState, firstMoves.moves[i][j].initialPosition, firstMoves.moves[i][j].movedPosition, colorType, firstMoves.moves[i][j].pieceType, firstMoves.moves[i][j].castling, firstMoves.moves[i][j].capturedPiece, 1);
                continue;

        }

        if (firstMoves.moves[i][j].castling)
        {
            if (isRookinCheck(boardState, colorType, firstMoves.moves[i][j].castling))
            {
                // dont recurse down
                updateBoardState(boardState, firstMoves.moves[i][j].initialPosition, firstMoves.moves[i][j].movedPosition, colorType, firstMoves.moves[i][j].pieceType, firstMoves.moves[i][j].castling, firstMoves.moves[i][j].capturedPiece, 1);
                continue;
            }
        }

/*
        if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
            score = mini(boardState, !colorType, recurseDepth - 1);
        else
            score = maxi(boardState, !colorType, recurseDepth - 1);
*/
        int alpha = -11111111;
        int beta = 11111111;


        if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
        {
            //score = firstMovesEval + alphaBetaMin(boardState, alpha, beta, !colorType, recurseDepth - 1);
            score = alphaBetaMin(boardState, alpha, beta, !colorType, recurseDepth - 1);

            //score += eval(boardState)/64; //bias the first Move

            if(score > maxScore)
            {
                maxScore = score;
                move = firstMoves.moves[i][j];
                move.boardEval = score;
            }
        }

        else
        {
            //score = firstMovesEval + alphaBetaMax(boardState, alpha, beta, !colorType, recurseDepth - 1);
            score = alphaBetaMax(boardState, alpha, beta, !colorType, recurseDepth - 1);
            //score -= eval(boardState)/64; //bias first Move

            if(score < minScore)
            {
                minScore = score;
                move = firstMoves.moves[i][j];
                move.boardEval = score;
            }
        }

                // undo the move
        updateBoardState(boardState, firstMoves.moves[i][j].initialPosition, firstMoves.moves[i][j].movedPosition, colorType, firstMoves.moves[i][j].pieceType, firstMoves.moves[i][j].castling, firstMoves.moves[i][j].capturedPiece, 1);


      }
    }

  }


  return move;
}




void swapMove(Move* a, Move* b)
{
  Move tmp;
  memcpy(&tmp, a, sizeof(Move));
  memcpy(a, b, sizeof(Move));
  memcpy(b, &tmp, sizeof(Move));
}



int calcCaptureScore(Move* move)
{
  int score = 0;

  int victim = move->capturedPiece;
  int attacker = -(move->pieceType - 8);

  // if there is a captured piece
  if(victim)
    score = victim * attacker;

  return score;
}


// only orders capture moves
void orderCaptureMoves(Moves* moves)
{
  int i, j, k;

  for(i = 0; i < NUM_PIECES; ++i)
  {
    for(j = 0; j < moves->numMoves[i]; ++j)
    {
      int startScore = calcCaptureScore(&moves->moves[i][j]);

      for(k = j + 1; k < moves->numMoves[i]; ++k)
      {
        int captureScore = calcCaptureScore(&moves->moves[i][k]);

        if(captureScore > startScore)
          swapMove(&moves->moves[i][j], &moves->moves[i][k]);
      }

    }
  }
}




void generateAllMoves(BoardState* boardState,
                      enum BitboardType colorType,
                      Moves* moves)
{
  generateAllPawnMoves(boardState, colorType, moves);
  generateAllRookMoves(boardState, colorType, moves);
  generateAllKnightMoves(boardState, colorType, moves);
  generateAllBishopMoves(boardState, colorType, moves);
  generateAllQueenMoves(boardState, colorType, moves);
  generateAllKingMoves(boardState, colorType, moves);

  orderCaptureMoves(moves);
}


void generateAllPawnMoves(BoardState* boardState,
                          enum BitboardType colorType,
                          Moves* moves)
{
  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_PAWN_POSITIONS] & boardState->boards[colorType];

  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;

    generatePawnMoves(boardState, isolatedPiece, colorType, moves);

    // reset ls1b
    pieces &= pieces - 1;
  }


}


void generatePawnMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType,
                         Moves* moves)
{
  generateAllSlidingMoves(boardState, isolatedPiece, BOARD_TYPE_ALL_PAWN_POSITIONS, colorType, moves);
  generateAllDiagonalMoves(boardState, isolatedPiece, BOARD_TYPE_ALL_PAWN_POSITIONS, colorType, moves);
}


void generateAllRookMoves(BoardState* boardState,
                           enum BitboardType colorType,
                          Moves* moves)
{
  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_ROOK_POSITIONS] & boardState->boards[colorType];

  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;

    generateRookMoves(boardState, isolatedPiece, colorType, moves);

    // reset ls1b
    pieces &= pieces - 1;
  }

}


void generateRookMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType,
                       Moves* moves)
{
  generateAllSlidingMoves(boardState, isolatedPiece, BOARD_TYPE_ALL_ROOK_POSITIONS, colorType, moves);
}


void generateAllKnightMoves(BoardState* boardState,
                           enum BitboardType colorType,
                            Moves* moves)
{
  Move move = { 0, 0, 0, 0, 0, 0 };
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

  for(i = 0; i < moves->numKnightMoves; ++i)
    moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][i].pieceType = BOARD_TYPE_ALL_KNIGHT_POSITIONS;

}


void generateKnightMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType,
                         Moves* moves)
{

  Move move = {0, 0, 0, 0, 0, 0};
	int i = 0;
    for( i = 0; i < sizeof(knightMoveGenerate) / sizeof(knightMoveGenerate[0]); ++i)
    {
        move = knightMoveGenerate[i](isolatedPiece, boardState, colorType);

            //if legal move
        if (move.movedPosition)
        {
            //capture move
            if (move.movedPosition & boardState->boards[!colorType])
            {
              move.capturedPiece = findCapturedPiece(boardState, move.movedPosition, colorType);
            } //capture move

            moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
        } //if legal move

    } // loop through knightMoveGenerate

} // generate knight moves



void generateAllBishopMoves(BoardState* boardState,
                           enum BitboardType colorType,
                            Moves* moves)
{
  Bitboard pieces = boardState->boards[BOARD_TYPE_ALL_BISHOP_POSITIONS] & boardState->boards[colorType];

  while(pieces)
  {
    Bitboard isolatedPiece = pieces & -pieces;

    generateBishopMoves(boardState, isolatedPiece, colorType, moves);

    // reset ls1b
    pieces &= pieces - 1;
  }

}


void generateBishopMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType,
                         Moves* moves)
{
  generateAllDiagonalMoves(boardState, isolatedPiece, BOARD_TYPE_ALL_BISHOP_POSITIONS, colorType, moves);
}


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


}




void generateQueenMoves(BoardState* boardState,
                        Bitboard isolatedPiece,
                        enum BitboardType colorType,
                        Moves* moves)
{
  generateAllSlidingMoves(boardState, isolatedPiece, BOARD_TYPE_ALL_QUEEN_POSITIONS, colorType, moves);
  generateAllDiagonalMoves(boardState, isolatedPiece, BOARD_TYPE_ALL_QUEEN_POSITIONS, colorType, moves);

}


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

}



void generateKingMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType,
                       Moves* moves)
{
  generateAllSlidingMoves(boardState, isolatedPiece, BOARD_TYPE_ALL_KING_POSITIONS, colorType, moves);
  generateAllDiagonalMoves(boardState, isolatedPiece, BOARD_TYPE_ALL_KING_POSITIONS, colorType, moves);

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
    move.movedPosition = initialPosition >> (9 * offset);

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
    move.movedPosition = initialPosition >> (7 * offset);

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
    Moves moves;
    moves.numMoves[0] = moves.numMoves[1] = moves.numMoves[2] =
    moves.numMoves[3] = moves.numMoves[4] = moves.numMoves[5] = 0;

    generateAllMoves(boardState, !colorType, &moves);
    int i = 0;
    int j = 0;

    for (i = 0; i < NUM_PIECES; i ++)
        for ( j = 0; j < moves.numMoves[i]; j ++)
            if (moves.moves[i][j].capturedPiece == BOARD_TYPE_ALL_KING_POSITIONS) //if the captured piece is the opponent's king
                return 1;

  return 0;
}





