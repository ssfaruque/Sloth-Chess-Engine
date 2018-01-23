
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * Description of Chess Engine HERE                                             *
 ********************************************************************************/

#include "slothChessEngine.h"


void initSlothChessEngine(SlothChessEngine* engine, BoardState* boardState, enum PlayerType playerType)
{
  engine->boardState = boardState;
  engine->playerType = playerType;
}


void cleanUpSlothEngine(SlothChessEngine* engine)
{
  // FILL THIS IN LATER
}



