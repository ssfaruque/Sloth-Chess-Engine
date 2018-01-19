
#include <stdio.h>
#include <stdlib.h>

#include "chessGame.h"


void initChessGame(ChessGame* chessGame)
{
  chessGame->running = 0;
  
  chessGame->boardState = (BoardState*) malloc(sizeof(BoardState));
  initBoardState(chessGame->boardState);
  
  chessGame->slothChessEngine = (SlothChessEngine*) malloc(sizeof(SlothChessEngine));
  initSlothChessEngine(chessGame->slothChessEngine, chessGame->boardState, PLAYER_TYPE_WHITE);
}


void runChessGame(ChessGame* chessGame)
{
  chessGame->running = 1;
  
  printBoardState(chessGame->boardState);
  
  printf("Size of board state is %d bytes\n", (int)sizeof(BoardState));
}


void cleanUpChessGame(ChessGame* chessGame)
{
}

