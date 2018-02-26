
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

#include "player.h"


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
  //playerPlayChess(chessGame);
	Move move;
	move.initialPosition = 0x0000000000000100;
	move.movedPosition = move.initialPosition << (8*5);
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, 0, BOARD_TYPE_ALL_PAWN_POSITIONS, 0);

	printBoardState(chessGame->boardState);

	move.initialPosition = 0x0001000000000000;
	move.movedPosition = move.initialPosition << 9;
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, 0, BOARD_TYPE_ALL_KNIGHT_POSITIONS, 0);
	printf("Queen Promotion:\n");
	printBoardState(chessGame->boardState);


	printf("Queen Promotion Undo:\n");
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, 0, BOARD_TYPE_ALL_KNIGHT_POSITIONS, 1);
	printBoardState(chessGame->boardState);


	
}

void cleanUpChessGame(ChessGame* chessGame)
{
}

