
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

          boardState->boards[capturedPiece] = (boardState->boards[capturedPiece]) | movedPiece;
          boardState->boards[!colorType] = ((boardState->boards[!colorType]) | movedPiece);
          boardState->boards[colorType] = ((boardState->boards[colorType] ^ movedPiece) | initialPiece);

          if (pieceType == capturedPiece)
            boardState->boards[pieceType] = (boardState->boards[pieceType] | initialPiece);
          else
            boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ movedPiece) | initialPiece);
      } // undo capture

      else if(capturedPiece) //capture
      {
          boardState->boards[capturedPiece] = (boardState->boards[capturedPiece]) ^ movedPiece;
          boardState->boards[!colorType] = ((boardState->boards[!colorType]) ^ movedPiece);


          boardState->boards[colorType] = ((boardState->boards[colorType] ^ initialPiece) | movedPiece);
          boardState->boards[pieceType] = ((boardState->boards[pieceType] ^ initialPiece) | movedPiece);

      } //capture

  return 0;
} //update board state



/******************************** Generate moves by pieces ********************************/


int findCapturedPiece(BoardState* boardState, Bitboard movedPosition, enum BitboardType colorType)
{
    for (int j = 0; j < NUM_PIECES; j++) //put captured Piece type into move
    {
        if (movedPosition & (boardState->boards[!colorType] & boardState->boards[j+2]))
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
  Move move = {0, 0, 0, 0, 0};
    int offset;

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

                if ((isolatedPiece & 0x000000000000ff00) != 0) //if in initial position, move up by 2
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

                if ((isolatedPiece & 0x00ff000000000000) != 0) //if in initial position
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

            for(int i = 0; i < sizeof(rookMoveGenerate) / sizeof(rookMoveGenerate[0]); ++i)
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
    Move move = {0, 0, 0, 0, 0};
    int offset;

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
            for(int i = 0; i < sizeof(bishopMoveGenerate) / sizeof(bishopMoveGenerate[0]); ++i)
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

            for(int i = 0; i < sizeof(bishopMoveGenerate) / sizeof(bishopMoveGenerate[0]); ++i)
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


int negaMax(BoardState* boardState,
            enum BitboardType colorType,
            int depth)
{
  if (depth == 0)
    return eval(boardState);

  int max = -111111111;

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
        updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, 0, moves.moves[i][j].capturedPiece, 0);

        if (isKingInCheck(boardState, colorType)) // the player's move leaves the player's king in check
        {
            //don't recurse down, undo and go to next move
                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, 0, moves.moves[i][j].capturedPiece, 1);
                continue;

        }

        score = -negaMax(boardState, !colorType, depth - 1);

        updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, 0, moves.moves[i][j].capturedPiece, 1);


            if(score >= max)
                max = score;



      } // if valid

    } //loop through num moves for each piece

  } //loop through num pieces

  return max;
} //negaMaz



// prototype
int mini(BoardState* boardState,
         enum BitboardType colorType, int depth);


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
                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, 0, moves.moves[i][j].capturedPiece, 0);

                if (isKingInCheck(boardState, colorType)) // the player's move leaves the player's king in check
                {
                    //don't recurse down, undo and go to next move
                    updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, 0, moves.moves[i][j].capturedPiece, 1);
                    continue;

                }

                score = mini(boardState, !colorType, depth - 1);

                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, 0, moves.moves[i][j].capturedPiece, 1);

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
                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, 0, moves.moves[i][j].capturedPiece, 0);

                if (isKingInCheck(boardState, colorType)) // the player's move leaves the player's king in check
                {
                    //don't recurse down, undo and go to next move
                    updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, 0, moves.moves[i][j].capturedPiece, 1);
                    continue;

                }

                score = maxi(boardState, !colorType, depth - 1);

                updateBoardState(boardState, moves.moves[i][j].initialPosition, moves.moves[i][j].movedPosition, colorType, moves.moves[i][j].pieceType, 0, moves.moves[i][j].capturedPiece, 1);

                if (score < min)
                    min = score;

            }
        }
    }

    return min;
}

Move generateMove(BoardState* boardState,
                  enum BitboardType colorType,
                  int recurseDepth)
{
  Move move = {0, 0, 0, 0, 0};

  int i;
  int j;

  int maxScore = -11111111;
  int minScore = 11111111;
    int score = 0;

  Moves firstMoves;
  firstMoves.numMoves[0] = firstMoves.numMoves[1] = firstMoves.numMoves[2] =
  firstMoves.numMoves[3] = firstMoves.numMoves[4] = firstMoves.numMoves[5] = 0;
  generateAllMoves(boardState, colorType, &firstMoves);
  
  
  if(colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
    recurseDepth = 3;
  
  
  for(i = 0; i < NUM_PIECES; ++i)
  {

    for(j = 0; j < firstMoves.numMoves[i]; ++j)
    {
      if(firstMoves.moves[i][j].initialPosition)
      {
        // do the move
        updateBoardState(boardState, firstMoves.moves[i][j].initialPosition, firstMoves.moves[i][j].movedPosition, colorType, firstMoves.moves[i][j].pieceType, 0, firstMoves.moves[i][j].capturedPiece, 0);

        if (isKingInCheck(boardState, colorType)) // the player's move leaves the player's king in check
        {
            //don't recurse down, undo and go to next move
                updateBoardState(boardState, firstMoves.moves[i][j].initialPosition, firstMoves.moves[i][j].movedPosition, colorType, firstMoves.moves[i][j].pieceType, 0, firstMoves.moves[i][j].capturedPiece, 1);
                continue;

        }

       // score = negaMax(boardState, !colorType, recurseDepth - 1);

        if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
            score = mini(boardState, !colorType, recurseDepth - 1);
        else
            score = maxi(boardState, !colorType, recurseDepth - 1);



        // undo the move
        updateBoardState(boardState, firstMoves.moves[i][j].initialPosition, firstMoves.moves[i][j].movedPosition, colorType, firstMoves.moves[i][j].pieceType, 0, firstMoves.moves[i][j].capturedPiece, 1);

        if (colorType == BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS)
        {

            if(score > maxScore)
            {
                maxScore = score;
                move = firstMoves.moves[i][j];
                move.boardEval = score;
            }
        }

        else
            if(score < minScore)
            {
                minScore = score;
                move = firstMoves.moves[i][j];
                move.boardEval = score;
            }


      }
    }

  }


  return move;
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

    Move move;

    for(int i = 0; i < sizeof(knightMoveGenerate) / sizeof(knightMoveGenerate[0]); ++i)
    {
        move = knightMoveGenerate[i](isolatedPiece, boardState, colorType);

            //if legal move
        if (move.movedPosition)
        {
            //capture move
            if (move.movedPosition & boardState->boards[!colorType])
            {
                for (int j = 0; j < NUM_PIECES; j++) // put captured Piece type into move
                {
                    if (move.movedPosition & boardState->boards[i + 2])
                    {
                        move.capturedPiece = i + 2;
                        break;
                    }
                }
            } //capture move

            moves->moves[BOARD_TYPE_ALL_KNIGHT_POSITIONS - 2][moves->numKnightMoves++] = move;
        } //if legal move

    } // loop through knightMoveGenerate

} // generate knight moves



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

    for (int i = 0; i < NUM_PIECES; i ++)
        for (int j = 0; j < moves.numMoves[i]; j ++)
            if (moves.moves[i][j].capturedPiece == BOARD_TYPE_ALL_KING_POSITIONS) //if the captured piece is the opponent's king
                return 1;

  return 0;
}





