//
//  player.h
//  ChessEngine
//
//  Created by Sahil Faruque on 2/22/18.
//  Copyright © 2018 Sahil Faruque. All rights reserved.
//

#ifndef player_h
#define player_h

#include <stdio.h>


#include "chessGame.h"

#include "bitboard.h"



void printBoardGUI(BoardState* boardState);

char getSymbol(enum BitboardType color, enum BitboardType pieceType);



void playerPlayChess(ChessGame* chessGame);




#endif /* player_h */
