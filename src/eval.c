
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * A Chess AI that can play chess                                               *
 ********************************************************************************/


#include "eval.h"

const int weights[] = {100, 500, 300, 325, 900, 5000};


int countNumPieces(Bitboard bitboard)
{
  int numPieces = 0;

  while(bitboard)
  {
    bitboard &= bitboard -1;
    ++numPieces;
  }

  return numPieces;
}


int materialEval(BoardState* boardState)
{

  const int NUM_PIECES = 6;

  int numWhitePieces[6] = {-1, -1, -1, -1, -1, -1};
  int numBlackPieces[6] = {-1, -1, -1, -1, -1, -1};

  int pieceType;  // index into the type of bitboard
  int index = 0;  // index into the number of pieces arrays

  for(pieceType = BOARD_TYPE_ALL_PAWN_POSITIONS;
      pieceType <= BOARD_TYPE_ALL_KING_POSITIONS;
      ++pieceType)
  {
    numWhitePieces[index] = countNumPieces(boardState->boards[BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS]
                                           & boardState->boards[pieceType]);
    numBlackPieces[index++] = countNumPieces(boardState->boards[BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS]
                                             & boardState->boards[pieceType]);
  }

  int score = 0;
  int i;

  for(i = 0; i < NUM_PIECES; ++i)
    score +=  weights[i] * (numWhitePieces[i] - numBlackPieces[i]);

  return score;
}


int centerControlEval(BoardState* boardState)
{
  Bitboard whitePiecesInCenter = boardState->boards[BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS] &
                                 0x0000001818000000;

  Bitboard blackPiecesInCenter = boardState->boards[BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS] &
                                 0x0000001818000000;

  int numWhitePieces = countNumPieces(whitePiecesInCenter);
  int numBlackPieces = countNumPieces(blackPiecesInCenter);

  const int CENTER_CONTROL_WEIGHT = 200;

  return CENTER_CONTROL_WEIGHT * (numWhitePieces - numBlackPieces);
}


int eval(BoardState* boardState)
{
  return materialEval(boardState) + centerControlEval(boardState);
}







