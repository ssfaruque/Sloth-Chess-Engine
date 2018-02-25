
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
	/****Test Black Castling King Side 

	Move move;
	move.initialPosition = 0x0010000000000000;
	move.movedPosition = move.initialPosition >> 8;
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, 0, 0, 0);

	printBoardState(chessGame->boardState);

	move.initialPosition = 0x2000000000000000;
	move.movedPosition = move.initialPosition >> 18;
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, BOARD_TYPE_ALL_BISHOP_POSITIONS, 0, 0, 0);

	printBoardState(chessGame->boardState);

	move.initialPosition = 0x4000000000000000;
	move.movedPosition = move.initialPosition >> 15;
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, BOARD_TYPE_ALL_KNIGHT_POSITIONS, 0, 0, 0);

	printBoardState(chessGame->boardState);

	move.initialPosition = 0x1000000000000000;
	move.movedPosition = move.initialPosition >> 8;
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, BOARD_TYPE_ALL_QUEEN_POSITIONS, 0, 0, 0);

	printBoardState(chessGame->boardState);


	move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, MAX_RECURSION_DEPTH);
	printf("move.castling: %d", move.castling);

	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, move.pieceType, move.castling, 0, 0);
	printBoardState(chessGame->boardState);
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, move.pieceType, move.castling, 0, 1);
	printBoardState(chessGame->boardState);
*/

	/****Test White Castling Queen Side 

	Move move;
	move.initialPosition = 0x0000000000001000;
	move.movedPosition = move.initialPosition << 8;
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, 0, 0, 0);

	printBoardState(chessGame->boardState);

	move.initialPosition = 0x0000000000000040;
	move.movedPosition = move.initialPosition << 17;
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_KNIGHT_POSITIONS, 0, 0, 0);

	printBoardState(chessGame->boardState);

	move.initialPosition = 0x0000000000000020;
	move.movedPosition = move.initialPosition << 14;
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_BISHOP_POSITIONS, 0, 0, 0);

	printBoardState(chessGame->boardState);

	move.initialPosition = 0x0000000000000010;
	move.movedPosition = move.initialPosition << 8;
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_QUEEN_POSITIONS, 0, 0, 0);

	printBoardState(chessGame->boardState);


	//move.initialPosition = 0x0000000000000080;
	//move.movedPosition = move.initialPosition >>3;
	//move.castling = QUEENS_SIDE;
	//updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_ROOK_POSITIONS, move.castling, 0, 0);


	//printBoardState(chessGame->boardState);
	// updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_ROOK_POSITIONS, move.castling, 0, 1);
	//printBoardState(chessGame->boardState);

	move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, MAX_RECURSION_DEPTH);
	printf("move.castling: %d", move.castling);

	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, move.pieceType, move.castling, 0, 0);
	printBoardState(chessGame->boardState);
	updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, move.pieceType, move.castling, 0, 1);
	printBoardState(chessGame->boardState);
	*/

  playerPlayChess(chessGame);

}

void cleanUpChessGame(ChessGame* chessGame)
{
}

