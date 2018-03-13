
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



  //setBoardStateWithFEN(engine, "r3k1nr/ppp3pp/3Np3/6B1/8/5q2/PPPQ3P/3RK2R w KQq");
}


void cleanUpSlothEngine(SlothChessEngine* engine)
{
  // FILL THIS IN LATER
}



