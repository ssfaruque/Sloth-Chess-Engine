
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * A Chess AI that can play chess                                               *
 ********************************************************************************/

#ifndef chessGame_h
#define chessGame_h

#include "bitboard.h"
#include "slothChessEngine.h"

typedef struct
{
  int running;
  BoardState* boardState;
  SlothChessEngine* slothChessEngine;
  
} ChessGame;


void initChessGame(ChessGame* chessGame);
void runChessGame(ChessGame* chessGame);
void cleanUpChessGame(ChessGame* chessGame);


#endif /* chessGame_h */
