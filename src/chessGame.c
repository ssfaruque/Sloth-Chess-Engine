
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
  //Move move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, 1);

  /*
  printBitboard(move.initialPosition);
  printBitboard(move.movedPosition);
  printBoardState(chessGame->boardState);*/
    Move move;
    move.initialPosition = 0x0000000000000100;
    move.movedPosition = move.initialPosition << 16;
    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, MOVE_TYPE_QUIET, 0);

    printBoardState(chessGame->boardState);

    move.initialPosition = 0x0002000000000000;
    move.movedPosition = move.initialPosition >> 16;
    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, MOVE_TYPE_QUIET, 0);

    printBoardState(chessGame->boardState);

    int capturedPiece;
    move.initialPosition = 0x0000000000000100 << 16;
    move.movedPosition = move.initialPosition << 9;
    capturedPiece = updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, MOVE_TYPE_CAPTURE, 0);

    printBoardState(chessGame->boardState);

    updateBoardState(chessGame->boardState, move.movedPosition, move.initialPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, MOVE_TYPE_CAPTURE, capturedPiece);
    printBoardState(chessGame->boardState);
}


void cleanUpChessGame(ChessGame* chessGame)
{
}

