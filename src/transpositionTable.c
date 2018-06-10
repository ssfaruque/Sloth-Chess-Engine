#include "transpositionTable.h"
#include "eval.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define NUM_TABLE_ENTRIES 8388608


typedef struct
{
    int eval;
    uint64_t hashVal;
} Entry;



typedef struct
{
    Entry* entries;
} TranspositionTable;

TranspositionTable table;



uint64_t randPieceSquareHashVals[2][6][64];


int findSquareNum(Bitboard isolatedPiece)
{
    int numShifts = 0;
    
    isolatedPiece = isolatedPiece >> 1;
    
    while(isolatedPiece)
    {
        numShifts++;
        isolatedPiece = isolatedPiece >> 1;
    }
    
    return numShifts;
}


uint64_t genHash(BoardState* boardState)
{
    int i;
    uint64_t hash = 0;
    
    for(i = BOARD_TYPE_ALL_PAWN_POSITIONS; i < NUM_BITBOARDS; ++i)
    {
        Bitboard whitePieces = boardState->boards[i] & boardState->boards[BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS];
        Bitboard blackPieces = boardState->boards[i] & boardState->boards[BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS];
        
        while(whitePieces)
        {
            Bitboard isolatedPiece = whitePieces & -whitePieces;
            
            int squareNum = findSquareNum(isolatedPiece);
            
            hash ^= randPieceSquareHashVals[BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS][i][squareNum];
            
            // reset ls1b
            whitePieces &= whitePieces - 1;
        }
        
        
        while(blackPieces)
        {
            Bitboard isolatedPiece = blackPieces & -blackPieces;
            
            int squareNum = findSquareNum(isolatedPiece);
            
            hash ^= randPieceSquareHashVals[BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS][i][squareNum];
            
            // reset ls1b
            blackPieces &= blackPieces - 1;
        }
    }
    
    return hash;
}


uint64_t genRand64()
{
    return (unsigned long long)(((double)rand() / (double)RAND_MAX) * 0xffffffffffffffff);
}


void initTransTable()
{
    static int initRandNums = 0;
    
    if(!initRandNums)
    {
        srand((unsigned int)time(NULL));
        
        int i, j, k;
        
        for(i = 0; i < 2; ++i)
        {
            for(j = 0; j < 6; ++j)
            {
                for(k = 0; k < 64; ++k)
                {
                    randPieceSquareHashVals[i][j][k] = genRand64();
                }
            }
        }
        
        table.entries = (Entry*) malloc(sizeof(Entry) * NUM_TABLE_ENTRIES);
        
        clearTable(table);
        
        initRandNums = 1;
        
    }
}


void addToTable(BoardState* boardState, int eval)
{
    uint64_t hashVal = genHash(boardState);
    int index = hashVal % NUM_TABLE_ENTRIES;
    
    table.entries[index].hashVal = hashVal;
    table.entries[index].eval = eval;
    
}


void clearTable()
{
    memset((char*) table.entries, 0xFF, sizeof(table.entries));
}


int findInTable(BoardState* boardState)
{
    uint64_t hashVal = genHash(boardState);
    int index = hashVal % NUM_TABLE_ENTRIES;
    
    if(table.entries[index].hashVal == hashVal)
    {
        
        return table.entries[index].eval;
    }
    
    
    return TABLE_ENTRY_INVALID;
}
