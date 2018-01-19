

#ifndef slothChessEngine_h
#define slothChessEngine_h

#include "bitboard.h"
#include "move.h"


typedef struct
{
  BoardState* boardState;
  enum PlayerType playerType;
} SlothChessEngine;


void initSlothChessEngine(SlothChessEngine* engine, BoardState* boardState, enum PlayerType playerType);
void cleanUpSlothEngine(SlothChessEngine* engine);




#endif /* slothChessEngine_h */
