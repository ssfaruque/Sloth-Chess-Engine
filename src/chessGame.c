
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
#include "eval.h"



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
  //Move move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, MAX_RECURSION_DEPTH);
  
  //printBoardState(chessGame->boardState);
  
  //printBitboard(move.initialPosition);
  //printBitboard(move.movedPosition);
  
  
  Move* moves = generateAllKnightMoves(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS);
  
  int i;
  
  for(i = 0; i < 100; ++i)
  {
    printf("initialPosition = %llu, movedPosition = %llu\n", moves[i].initialPosition, moves[i].movedPosition);
  }
  
  
}


void cleanUpChessGame(ChessGame* chessGame)
{
}

