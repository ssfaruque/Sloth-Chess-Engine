
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * Description of Chess Engine HERE                                             *
 ********************************************************************************/

#include "slothChessEngine.h"
#include "player.h"

void initSlothChessEngine(SlothChessEngine* engine, BoardState* boardState, enum BitboardType playerType)
{
  engine->boardState = boardState;
  engine->playerType = playerType;
  engine->turn       = 0;

    //initial
setBoardStateWithFEN(engine, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
  //setBoardStateWithFEN(engine, "8/3R1pk1/P6p/8/5P2/1n6/4K2P/Q7 w -");
}


void setEngineColor(SlothChessEngine* engine, enum BitboardType playerType)
{
	engine->playerType = playerType;
}




void cleanUpSlothEngine(SlothChessEngine* engine)
{
  // FILL THIS IN LATER
}



