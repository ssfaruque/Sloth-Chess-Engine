//
//  xboard.c
//  ChessEngine
//
//  Created by Sahil Faruque on 4/7/18.
//  Copyright © 2018 Sahil Faruque. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#include "xboard.h"



void processXboardCmd(ChessGame* chessGame, const char* cmd, FILE* file)
{
    /* user entered a move */
    if(cmd[1] >= '1' && cmd[1] <= '8')
    {
        int beforeCol = cmd[0] - 'a' + 1;
        int beforeRow = cmd[1] - '0';
        int afterRow = cmd[2] - 'a' + 1;
        int afterCol = cmd[3] - '0';
        
        
        Bitboard initialPiece = ((int64_t)1) << ((beforeRow  - 1) * 8 + (8 - beforeCol));
        Bitboard movedPiece = ((int64_t)1) << ((afterRow  - 1) * 8 + (8 - afterCol));
        
        Move move;
        move.initialPosition = initialPiece;
        move.movedPosition = movedPiece;
        move.castling = 0;
        move.enpassant = 0;
        move.pieceType = findCapturedPiece(chessGame->boardState, initialPiece, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS);
        move.capturedPiece = findCapturedPiece(chessGame->boardState, movedPiece, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS);
        
        updateBoardState(chessGame->boardState, initialPiece, movedPiece, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, move.pieceType, move.castling, move.enpassant, move.capturedPiece, 0);
        
        move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, MAX_RECURSION_DEPTH);
        
        updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, move.pieceType, move.castling, move.enpassant, move.capturedPiece, 0);
        
        beforeRow = findRow(move.initialPosition);
        beforeCol = findCol(move.initialPosition);
        
        afterRow = findRow(move.movedPosition);
        afterCol = findCol(move.movedPosition);
        
        char sendMove[50];
        sendMove[0] = 'm';
        sendMove[1] = 'o';
        sendMove[2] = 'v';
        sendMove[3] = 'e';
        sendMove[4] = ' ';
        
        sendMove[5] = 'a' + beforeCol - 1;
        sendMove[6] = '0' + beforeRow;
        
        sendMove[7] = 'a' + afterCol - 1;
        sendMove[8] = '0' + afterRow;
        //sendMove[9] = '\0';
        
        fprintf(file, "Engine: %s\n", sendMove);
        printf("%s\n", sendMove);
        
        
    }
    
    else if(strcmp(cmd, "xboard") == 0)
    {
        printf("\n");
        
    }
    
    else if(strcmp(cmd, "protover 2") == 0)
    {
        printf("feature usermove=0 time=0 done=1\n");
    }
    
    else if(strcmp(cmd, "new") == 0)
    {
        initChessGame(chessGame);
        chessGame->running = 1;
        
        fprintf(file, "Engine: STARTING NEW GAME FOR ENGINE!\n");
    }
    
    
    fflush(stdout);
}



void playChessWithXboard(ChessGame* chessGame)
{
    const int BUFFER_SIZE = 512;
    char buffer[512];
    
    FILE* file = fopen("xboard_debug.txt", "w");
    fclose(file);
    
    chessGame->running = 1;
    
    while(chessGame->running)
    {
        fgets(buffer, BUFFER_SIZE, stdin);
        
        buffer[strlen(buffer) - 1] = '\0';
        
        file = fopen("xboard_debug.txt", "a");
        fprintf(file, "Xboard: %s\n", buffer);
        
        processXboardCmd(chessGame, buffer, file);
        
        fclose(file);
    }
    
}



