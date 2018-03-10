
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * Description of Chess Engine HERE                                             *
 ********************************************************************************/

#include "slothChessEngine.h"
#include "player.h"

void initSlothChessEngine(SlothChessEngine* engine, BoardState* boardState, enum PlayerType playerType)
{
  engine->boardState = boardState;
  engine->playerType = playerType;
  engine->turn       = 0;



  setBoardStateWithFEN(engine, "rnb1kbnr/ppppq1pp/8/4N3/3Pp3/8/PPP2PPP/RNBQKB1R");
}


void cleanUpSlothEngine(SlothChessEngine* engine)
{
  // FILL THIS IN LATER
}



