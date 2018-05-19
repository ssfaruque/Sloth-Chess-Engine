//
//  transpositionTable.h
//  ChessEngine
//
//  Created by Sahil Faruque on 5/8/18.
//  Copyright © 2018 Sahil Faruque. All rights reserved.
//

#ifndef transpositionTable_h
#define transpositionTable_h

#include <stdio.h>

#include "bitboard.h"

#include "move.h"

#define TABLE_ENTRY_INVALID 0xFFFFFFFF


void initTransTable();

void addToTable(BoardState* boardState, int eval);

void clearTable();

int findInTable(BoardState* boardState);

uint64_t genHash(BoardState* boardState);



#endif /* transpositionTable_h */
