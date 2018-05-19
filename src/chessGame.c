
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

#include "transpositionTable.h"


void initChessGame(ChessGame* chessGame)
{    
  chessGame->running = 0;

  chessGame->boardState = (BoardState*) malloc(sizeof(BoardState));

  initBoardState(chessGame->boardState);

  chessGame->slothChessEngine = (SlothChessEngine*) malloc(sizeof(SlothChessEngine));
  initSlothChessEngine(chessGame->slothChessEngine, chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS);
  //printBoardGUI(chessGame->boardState);
}


void runChessGame(ChessGame* chessGame)
{
  playerPlayChess(chessGame);

  //setBoardStateWithFEN(chessGame->slothChessEngine, "rnb1k1nr/pp1p1ppp/2p5/4P3/1q2PP2/3Q2PN/PKP4P/RN3B1R w kq");

  Move move;

  printBoardGUI(chessGame->boardState);

  while (1)
  {

             printf("White Ply:\n");
  move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, MAX_RECURSION_DEPTH);
  updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, move.pieceType, move.castling, move.enpassant, move.capturedPiece, 0);

                  printf("White Left Castle: %d.\n", chessGame-> boardState->castlingFlags[0][WHITE_QUEENS_SIDE]);
    printf("White Right Castle: %d.\n", chessGame->boardState->castlingFlags[0][WHITE_KINGS_SIDE]);
      printf("Black Left Castle: %d.\n", chessGame->boardState->castlingFlags[0][BLACK_QUEENS_SIDE]);
        printf("Black Right Castle: %d.\n", chessGame->boardState->castlingFlags[0][BLACK_KINGS_SIDE]);

  printBoardGUI(chessGame->boardState);
  printBoardState(chessGame->boardState);

    generateFEN(chessGame->slothChessEngine);
    printf("FEN: %s\n", chessGame->slothChessEngine->FEN);


    printf("Black Ply:\n");
  move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, MAX_RECURSION_DEPTH);
  updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, move.pieceType, move.castling, move.enpassant, move.capturedPiece, 0);


                  printf("White Left Castle: %d.\n", chessGame->boardState->castlingFlags[0][WHITE_QUEENS_SIDE]);
    printf("White Right Castle: %d.\n", chessGame->boardState->castlingFlags[0][WHITE_KINGS_SIDE]);
      printf("Black Left Castle: %d.\n", chessGame->boardState->castlingFlags[0][BLACK_QUEENS_SIDE]);
        printf("Black Right Castle: %d.\n", chessGame->boardState->castlingFlags[0][BLACK_KINGS_SIDE]);

    printBoardGUI(chessGame->boardState);
  printBoardState(chessGame->boardState);

   generateFEN(chessGame->slothChessEngine);
    printf("FEN: %s\n", chessGame->slothChessEngine->FEN);
  /*-----------------------------------------------*/




  }
    

  /*
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
*/


}

void cleanUpChessGame(ChessGame* chessGame)
{
}

