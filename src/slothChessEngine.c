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
    
}


void setEngineColor(SlothChessEngine* engine, enum BitboardType playerType)
{
    engine->playerType = playerType;
}


