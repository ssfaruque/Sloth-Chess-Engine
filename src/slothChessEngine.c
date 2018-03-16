
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
setBoardStateWithFEN(engine, "rnb1kbnr/ppp3pp/8/8/2N2p2/3P4/PP1P1PqP/RNBQKR2 w Qkq");
 // setBoardStateWithFEN(engine, "rn3k1r/ppp1nppp/8/1B2p3/4P1b1/B1P2N2/P1P2PPP/R2K3R w -");
}


void setEngineColor(SlothChessEngine* engine, enum BitboardType playerType)
{
	engine->playerType = playerType;
}




void cleanUpSlothEngine(SlothChessEngine* engine)
{
  // FILL THIS IN LATER
}



