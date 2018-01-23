
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * Description of Chess Engine HERE                                             *
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "chessGame.h"
#include "move.h"


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
}


void cleanUpChessGame(ChessGame* chessGame)
{
}

