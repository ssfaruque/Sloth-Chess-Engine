
#ifndef chessGame_h
#define chessGame_h

#include "bitboard.h"
#include "slothChessEngine.h"

typedef struct
{
  int running;
  BoardState* boardState;
  SlothChessEngine* slothChessEngine;
  
} ChessGame;


void initChessGame(ChessGame* chessGame);
void runChessGame(ChessGame* chessGame);
void cleanUpChessGame(ChessGame* chessGame);


#endif /* chessGame_h */
