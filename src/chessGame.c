
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
  int i = 0;

  while(1)
  {
    Move move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, MAX_RECURSION_DEPTH);

    
    if(isKingInCheck(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS))
    {
      if(move.initialPosition == 0)
      {
        printf("Checkmate, white loses\n");
        break;
      }
    }
    
    
    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, move.pieceType, 0, move.capturedPiece, 0);

    i++;
    chessGame->slothChessEngine->turn++;


    printf("Turn White %d\n", chessGame->slothChessEngine->turn);
    printf("Score = %d\n", move.boardEval);
    printf("Piece type: %d\n", move.pieceType);
    printf("Captured piece type: %d\n", move.capturedPiece);

    
    printBoardState(chessGame->boardState);
    

    
    

    


    move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, MAX_RECURSION_DEPTH);
    
    
    if(isKingInCheck(chessGame->boardState, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS))
    {
      if(move.initialPosition == 0)
      {
        printf("Checkmate, black loses\n");
        break;
      }
    }
    
    

    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, move.pieceType, 0, move.capturedPiece, 0);

     chessGame->slothChessEngine->turn++;

    printf("Turn Black: %d\n", chessGame->slothChessEngine->turn);
    printf("Score = %d\n", move.boardEval);
        printf("Piece type: %d\n", move.pieceType);
    printf("Captured piece type: %d\n", move.capturedPiece);
    printBoardState(chessGame->boardState);

  }

}


void cleanUpChessGame(ChessGame* chessGame)
{
}

