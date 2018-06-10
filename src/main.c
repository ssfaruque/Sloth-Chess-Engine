#include <stdio.h>

#include "chessGame.h"
#include "bitboard.h"
#include "slothChessEngine.h"



int main(int argc, const char * argv[])
{
    ChessGame chessGame;
    initChessGame(&chessGame);
    runChessGame(&chessGame);
    
    return 0;
}
