
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * A Chess AI that can play chess                                               *
 ********************************************************************************/


#include "eval.h"

const int weights[] = {100, 500, 300, 325, 900, 5000};





const int pawnWhitePST[] =
{
  0,  0,  0,  0,  0,  0,  0,  0,
  50, 50, 50, 50, 50, 50, 50, 50,
  10, 10, 20, 30, 30, 20, 10, 10,
  5,  5, 10, 25, 25, 10,  5,  5,
  0,  0,  0, 20, 20,  0,  0,  0,
  5, -5,-10,  0,  0,-10, -5,  5,
  5, 10, 10,-20,-20, 10, 10,  5,
  0,  0,  0,  0,  0,  0,  0,  0
};

const int pawnBlackPST[] =
{
  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10,-20,-20, 10, 10,  5,
  5, -5,-10,  0,  0,-10, -5,  5,
  0,  0,  0, 20, 20,  0,  0,  0,
  5,  5, 10, 25, 25, 10,  5,  5,
  10, 10, 20, 30, 30, 20, 10, 10,
  50, 50, 50, 50, 50, 50, 50, 50,
  0,  0,  0,  0,  0,  0,  0,  0
};


const int rookWhitePST [] =
{
  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10, 10, 10, 10, 10,  5,
  -5,  0,  0,  0,  0,  0,  0, -5,
  -5,  0,  0,  0,  0,  0,  0, -5,
  -5,  0,  0,  0,  0,  0,  0, -5,
  -5,  0,  0,  0,  0,  0,  0, -5,
  -5,  0,  0,  0,  0,  0,  0, -5,
  0,  0,  0,  5,  5,  0,  0,  0
};

const int rookBlackPST [] =
{
  0,  0,  0,  5,  5,  0,  0,  0,
  -5,  0,  0,  0,  0,  0,  0, -5,
  -5,  0,  0,  0,  0,  0,  0, -5,
  -5,  0,  0,  0,  0,  0,  0, -5,
  -5,  0,  0,  0,  0,  0,  0, -5,
  -5,  0,  0,  0,  0,  0,  0, -5,
  5, 10, 10, 10, 10, 10, 10,  5,
  0,  0,  0,  0,  0,  300,  0,  0
};


const int knightWhitePST [] =
{
  -50,-40,-30,-30,-30,-30,-40,-50,
  -40,-20,  0,  0,  0,  0,-20,-40,
  -30,  0, 10, 15, 15, 10,  0,-30,
  -30,  5, 15, 20, 20, 15,  5,-30,
  -30,  0, 15, 20, 20, 15,  0,-30,
  -30,  5, 10, 15, 15, 10,  5,-30,
  -40,-20,  0,  5,  5,  0,-20,-40,
  -50,-40,-30,-30,-30,-30,-40,-50
};

const int knightBlackPST [] =
{
  -50,-40,-30,-30,-30,-30,-40,-50,
  -40,-20,  0,  0,  0,  0,-20,-40,
  -30,  0, 10, 15, 15, 10,  0,-30,
  -30,  5, 15, 20, 20, 15,  5,-30,
  -30,  0, 15, 20, 20, 15,  0,-30,
  -30,  5, 10, 15, 15, 10,  5,-30,
  -40,-20,  0,  5,  5,  0,-20,-40,
  -50,-40,-30,-30,-30,-30,-40,-50
};
const int bishopWhitePST [] =
{
  -20,-10,-10,-10,-10,-10,-10,-20,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -10,  0,  5, 10, 10,  5,  0,-10,
  -10,  5,  5, 10, 10,  5,  5,-10,
  -10,  0, 10, 10, 10, 10,  0,-10,
  -10, 10, 10, 10, 10, 10, 10,-10,
  -10,  5,  0,  0,  0,  0,  5,-10,
  -20,-10,-10,-10,-10,-10,-10,-20
};

const int bishopBlackPST [] =
{
  -20,-10,-10,-10,-10,-10,-10,-20,
  -10,  5,  0,  0,  0,  0,  5,-10,
  -10, 10, 10, 10, 10, 10, 10,-10,
  -10,  0, 10, 10, 10, 10,  0,-10,
  -10,  5,  5, 10, 10,  5,  5,-10,
  -10,  0,  5, 10, 10,  5,  0,-10,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -20,-10,-10,-10,-10,-10,-10,-20
};
const int queenWhitePST [] =
{
  -20,-10,-10, -5, -5,-10,-10,-20,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -10,  0,  5,  5,  5,  5,  0,-10,
  -5,  0,  5,  5,  5,  5,  0, -5,
  0,  0,  5,  5,  5,  5,  0, -5,
  -10,  5,  5,  5,  5,  5,  0,-10,
  -10,  0,  5,  0,  0,  0,  0,-10,
  -20,-10,-10, -5, -5,-10,-10,-20
};

const int queenBlackPST [] =
{
  -20,-10,-10, -5, -5,-10,-10,-20,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -10,  5,  5,  5,  5,  5,  0,-10,
  -5,  0,  5,  5,  5,  5,  0, -5,
  0,  0,  5,  5,  5,  5,  0, -5,
  -10,  0,  5,  5,  5,  5,  0,-10,
  -10,  0,  5,  0,  0,  0,  0,-10,
  -20,-10,-10, -5, -5,-10,-10,-20
};

const int kingWhitePST [] =
{
  //king middle game
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -20,-30,-30,-40,-40,-30,-30,-20,
  -10,-20,-20,-20,-20,-20,-20,-10,
  20, 20,  0,  0,  0,  0, 20, 20,
  20, 30, 10,  0,  0, 10, 300, 20
};

const int kingBlackPST [] =
{
  //king middle game
   20, 30, 10,  0,  0, 10, 30, 20,
   20, 20,  0,  0,  0,  0, 20, 20,
  -10,-20,-20,-20,-20,-20,-20,-10,
  -20,-30,-30,-40,-40,-30,-30,-20,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
};


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




int spaceScore(enum BitboardType colorType, int space, int pieceType)
{

    if (colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
    {

        switch(pieceType)
        {
            case BOARD_TYPE_ALL_PAWN_POSITIONS:
            return pawnBlackPST[space];

            case BOARD_TYPE_ALL_ROOK_POSITIONS:
            return rookBlackPST[space];

            case BOARD_TYPE_ALL_KNIGHT_POSITIONS:
            return kingBlackPST[space];

            case BOARD_TYPE_ALL_BISHOP_POSITIONS:
            return bishopBlackPST[space];

            case BOARD_TYPE_ALL_QUEEN_POSITIONS:
            return queenBlackPST[space];

            case BOARD_TYPE_ALL_KING_POSITIONS:
            return kingBlackPST[space];

            default:
            return 0;
        } // switch
    } //if black

    else //white
    {
        switch(pieceType)
        {
            case BOARD_TYPE_ALL_PAWN_POSITIONS:
            return pawnWhitePST[space];

            case BOARD_TYPE_ALL_ROOK_POSITIONS:
            return rookWhitePST[space];

            case BOARD_TYPE_ALL_KNIGHT_POSITIONS:
            return kingWhitePST[space];

            case BOARD_TYPE_ALL_BISHOP_POSITIONS:
            return bishopWhitePST[space];

            case BOARD_TYPE_ALL_QUEEN_POSITIONS:
            return queenWhitePST[space];

            case BOARD_TYPE_ALL_KING_POSITIONS:
            return kingWhitePST[space];

            default:
            return 0;


            } //switch
    } //if white

}



// Below function is from chess programming wiki

// Returns position of the only set bit in 'n'
unsigned int findPosition(uint64_t n, int colorType)
{
  uint64_t i = 0x8000000000000000, pos = 0;

  // Iterate through bits of n till we find a set bit
  // i&n will be non-zero only when 'i' and 'n' have a set bit
  // at same position
  while (!(i & n))
  {
    // Unset current bit and set the next bit in 'i'
    i = i >> 1;

    // increment position
    ++pos;

    //printf("n = %d \n", n);
  }


//  if(colorType == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
 //   return pos - 1;

  //else
    //return (56-((pos-1)/8)*8 + (pos-1)%8);
    return pos;
    //return pos;
}






int pieceSquareEval(BoardState* boardState)
{
  int score = 0;

  int pieceType;

  for(pieceType = 2; pieceType <= BOARD_TYPE_ALL_KING_POSITIONS; ++pieceType)
  {
    Bitboard whitePieces = boardState->boards[pieceType] & boardState->boards[BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS];
    Bitboard blackPieces = boardState->boards[pieceType] & boardState->boards[BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS];

    while(whitePieces)
    {
      Bitboard isolatedPiece = whitePieces & -whitePieces;

      int space = findPosition(isolatedPiece, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS);

      score += spaceScore(BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, space, pieceType);

      // reset ls1b
      whitePieces &= whitePieces - 1;
    }


    while(blackPieces)
    {
      Bitboard isolatedPiece = blackPieces & -blackPieces;

      int space = findPosition(isolatedPiece, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS);

      score += -spaceScore(BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, space, pieceType);

      // reset ls1b
      blackPieces &= blackPieces - 1;
    }


  }

  return score;
}



int bishopPairEval(BoardState* boardState)
{
  int score = 0;


  return score;
}


int eval(BoardState* boardState)
{
  return materialEval(boardState) + centerControlEval(boardState) + pieceSquareEval(boardState);
}







