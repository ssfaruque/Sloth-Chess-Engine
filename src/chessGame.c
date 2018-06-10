#include <stdio.h>
#include <stdlib.h>

#include "chessGame.h"
#include "move.h"
#include "eval.h"

#include "player.h"

#include "transpositionTable.h"


void initChessGame(ChessGame* chessGame)
{
    chessGame->running = 0;
    
    chessGame->boardState = (BoardState*) malloc(sizeof(BoardState));
    
    initBoardState(chessGame->boardState);
    
    chessGame->slothChessEngine = (SlothChessEngine*) malloc(sizeof(SlothChessEngine));
    initSlothChessEngine(chessGame->slothChessEngine, chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS);
}


void runChessGame(ChessGame* chessGame)
{
    playerPlayChess(chessGame);
}

