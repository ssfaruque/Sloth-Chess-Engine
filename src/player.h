#ifndef player_h
#define player_h

#include <stdio.h>

#include "chessGame.h"
#include "bitboard.h"


void printBoardGUI(BoardState* boardState);

char getSymbol(enum BitboardType color, enum BitboardType pieceType);

void playerPlayChess(ChessGame* chessGame);

void generateFEN(SlothChessEngine* engine);

void setBoardStateWithFEN(SlothChessEngine* engine, char* FEN);


#endif /* player_h */
