#ifndef slothChessEngine_h
#define slothChessEngine_h

#include "bitboard.h"
#include "transpositionTable.h"

#define MAX_FEN_LENGTH 100


/**
 *  @brief The chess engine that contains all necessary components to generate a move
 */
typedef struct
{
    BoardState* boardState;         /**< The current state of the game  */
    char FEN[MAX_FEN_LENGTH];
    enum BitboardType playerType;     /**< The color that the chess engine is currently playing as  */
    int turn;
} SlothChessEngine;





/**
 *  @brief Initializes the chess engine with the appropriate
 *  parameters so that it can start playing
 *  @param engine The uninitialized engine
 *  @param boardState The starting state of the chess game
 *  @param playerType The color that the chess engine is going to play as
 *  @return void
 */
void initSlothChessEngine(SlothChessEngine* engine, BoardState* boardState, enum BitboardType playerType);

void setEngineColor(SlothChessEngine* engine, enum BitboardType playerType);



#endif /* slothChessEngine_h */
