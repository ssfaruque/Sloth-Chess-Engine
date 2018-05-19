
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
  initTransTable();

    //initial
    setBoardStateWithFEN(engine, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");
//setBoardStateWithFEN(engine, "rnb1kbnr/pp2qppp/2p5/3p4/3Pp3/3BPN2/PPP2PPP/RNBQK2R w KQkq");

}


void setEngineColor(SlothChessEngine* engine, enum BitboardType playerType)
{
	engine->playerType = playerType;
}




void cleanUpSlothEngine(SlothChessEngine* engine)
{
  // FILL THIS IN LATER
}



