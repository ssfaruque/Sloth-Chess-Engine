
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
/*
    Move move;

    move.initialPosition = 0x0000000000000008;
    move.movedPosition = move.initialPosition << 24;
    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_KING_POSITIONS, 0, 0, 0);

    printBoardState(chessGame->boardState);
    int isKing = isKingInCheck(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS);
    printf("King in Check %d", isKing);

    move.initialPosition = 0x0800000000000000;
    move.movedPosition = move.initialPosition >> 24;
    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, BOARD_TYPE_ALL_KING_POSITIONS, 0, 0, 0);

    printBoardState(chessGame->boardState);
    isKing = isKingInCheck(chessGame->boardState, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS);
    printf("King in Check %d", isKing);

    move.initialPosition = 0x0010000000000000;
    move.movedPosition = move.initialPosition >> 8;
    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, 0, 0, 0);
 printBoardState(chessGame->boardState);
    isKing = isKingInCheck(chessGame->boardState, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS);
    printf("King in Check %d", isKing);


    move.initialPosition = 0x0000000000000008 << 24;
    move.movedPosition = move.initialPosition << 8;
    move.capturedPiece = BOARD_TYPE_ALL_KING_POSITIONS;
    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_KING_POSITIONS, 0, move.capturedPiece, 0);

        printBoardState(chessGame->boardState);
    isKing = isKingInCheck(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS);
    printf("King in Check %d", isKing);

        updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_KING_POSITIONS, 0, move.capturedPiece, 1);
printBoardState(chessGame->boardState);
    isKing = isKingInCheck(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS);
    printf("King in Check %d", isKing);
*/

  /*  move.initialPosition = 0x2000000000000000;
    move.movedPosition = move.initialPosition >> 22;
    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, BOARD_TYPE_ALL_BISHOP_POSITIONS, 0, 0, 0);

    printBoardState(chessGame->boardState);

    move.initialPosition = 0x0000000000001000;
    move.movedPosition = move.initialPosition << 16;
    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, 0, 0, 0);


    printBoardState(chessGame->boardState);
    isKing = isKingInCheck(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS);
    printf("King in Check %d", isKing);

    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, BOARD_TYPE_ALL_PAWN_POSITIONS, 0, 0, 1);
    printBoardState(chessGame->boardState);
    isKing = isKingInCheck(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS);
    printf("King in Check %d", isKing);

*/





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


   // move.initialPosition = 0x0000000000000080;
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

  /*
  int i = 0;

  while(1)
  {
    Move move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, MAX_RECURSION_DEPTH);

    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, move.pieceType, 0, move.capturedPiece, 0);

    i++;
    chessGame->slothChessEngine->turn++;


    printf("Turn White %d\n", chessGame->slothChessEngine->turn);
    printf("Score = %d\n", move.boardEval);
    printf("Piece type: %d\n", move.pieceType);
    printf("Captured piece type: %d\n", move.capturedPiece);

    printBoardState(chessGame->boardState);
if (move.castling)
{
    printf("CASTLING MOVE WAS MADE");
    break;
}


    move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, MAX_RECURSION_DEPTH);

    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, move.pieceType, 0, move.capturedPiece, 0);

     chessGame->slothChessEngine->turn++;

    printf("Turn Black: %d\n", chessGame->slothChessEngine->turn);
    printf("Score = %d\n", move.boardEval);
        printf("Piece type: %d\n", move.pieceType);
    printf("Captured piece type: %d\n", move.capturedPiece);
    printBoardState(chessGame->boardState);

    if (move.castling)
{
    printf("CASTLING MOVE WAS MADE");
    break;
}

  }
*/
}

void cleanUpChessGame(ChessGame* chessGame)
{
}

