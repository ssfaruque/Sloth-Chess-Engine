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

void setEngineColor(SlothChessEngine* engine, enum BitboardType playerType)
{
	engine->playerType = playerType;
}



void processXboardCmd(ChessGame* chessGame, const char* cmd, FILE* file)
{
	/* user entered a move */
	if (cmd[1] >= '1' && cmd[1] <= '8' || strcmp(cmd, "go") == 0)
	{
		int beforeCol = cmd[0] - 'a' + 1;
		int beforeRow = cmd[1] - '0';
		int afterCol = cmd[2] - 'a' + 1;
		int afterRow = cmd[3] - '0';

		Move move;
		Bitboard initialPiece = ((int64_t)1) << ((beforeRow - 1) * 8 + (8 - beforeCol));
		Bitboard movedPiece = ((int64_t)1) << ((afterRow - 1) * 8 + (8 - afterCol));

		int color = chessGame->slothChessEngine->playerType;

		/* pawn promotion (NOT FINISHED)*/
		if (strlen(cmd) == 5)
		{
			char pieceChar = cmd[4];
			int promotedPiece = -1;

			switch (promotedPiece)
			{
			case 'n':
				promotedPiece = BOARD_TYPE_ALL_KNIGHT_POSITIONS;
				break;

			case 'b':
				promotedPiece = BOARD_TYPE_ALL_BISHOP_POSITIONS;
				break;

			case 'r':
				promotedPiece = BOARD_TYPE_ALL_ROOK_POSITIONS;
				break;

			case 'q':
				promotedPiece = BOARD_TYPE_ALL_QUEEN_POSITIONS;
				break;
			}
		}


		/* castling */
		else if (getPieceType(initialPiece, color, chessGame->boardState) == BOARD_TYPE_ALL_KING_POSITIONS)
		{

			if (strcmp(cmd, "e1g1") == 0)		/* white king's side */
			{
				move.initialPosition = initialPiece;
				move.movedPosition = movedPiece;
				move.castling = KINGS_SIDE;
				move.enpassant = 0;
				move.pieceType = BOARD_TYPE_ALL_KING_POSITIONS;
				move.capturedPiece = 0;
				chessGame->boardState->castlingFlags[0][WHITE_KINGS_SIDE] = 0;
				fprintf(file, "Xboard: Performed castling!\n");
			}

			else if (strcmp(cmd, "e1c1") == 0)	/* white queen's side */
			{
				move.initialPosition = initialPiece;
				move.movedPosition = movedPiece;
				move.castling = QUEENS_SIDE;
				move.enpassant = 0;
				move.pieceType = BOARD_TYPE_ALL_KING_POSITIONS;
				move.capturedPiece = 0;
				chessGame->boardState->castlingFlags[0][WHITE_QUEENS_SIDE] = 0;
				fprintf(file, "Xboard: Performed castling!\n");
			}

			else if (strcmp(cmd, "e8g8") == 0)	/* black king's side */
			{
				move.initialPosition = initialPiece;
				move.movedPosition = movedPiece;
				move.castling = KINGS_SIDE;
				move.enpassant = 0;
				move.pieceType = BOARD_TYPE_ALL_KING_POSITIONS;
				move.capturedPiece = 0;
				chessGame->boardState->castlingFlags[0][BLACK_KINGS_SIDE] = 0;
				fprintf(file, "Xboard: Performed castling!\n");
			}

			else if (strcmp(cmd, "e8c8") == 0)	/* black queen's side */
			{
				move.initialPosition = initialPiece;
				move.movedPosition = movedPiece;
				move.castling = QUEENS_SIDE;
				move.enpassant = 0;
				move.pieceType = BOARD_TYPE_ALL_KING_POSITIONS;
				move.capturedPiece = 0;
				chessGame->boardState->castlingFlags[0][BLACK_QUEENS_SIDE] = 0;
				fprintf(file, "Xboard: Performed castling!\n");
			}
		}
		/* normal move */
		else
		{
			move.initialPosition = initialPiece;
			move.movedPosition = movedPiece;
			move.castling = 0;
			move.enpassant = 0;
			move.pieceType = getPieceType(initialPiece, color, chessGame->boardState);
			move.capturedPiece = findCapturedPiece(chessGame->boardState, movedPiece, color);
		}

		//update castling flags if user moved a king
		if (move.pieceType == BOARD_TYPE_ALL_KING_POSITIONS)
		{
			if (color == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
			{
				chessGame->boardState->castlingFlags[0][BLACK_QUEENS_SIDE] = 0;
				chessGame->boardState->castlingFlags[0][BLACK_KINGS_SIDE] = 0;

			}

			else
			{
				chessGame->boardState->castlingFlags[0][WHITE_QUEENS_SIDE] = 0;
				chessGame->boardState->castlingFlags[0][WHITE_KINGS_SIDE] = 0;
			}
		}

		//check if user moved rook to update flags

		if (move.pieceType == BOARD_TYPE_ALL_ROOK_POSITIONS)
		{
			if (color == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
			{
				if (move.initialPosition == 0x8000000000000000)
					chessGame->boardState->castlingFlags[0][BLACK_QUEENS_SIDE] = 0;
				else if (move.initialPosition == 0x0100000000000000)
					chessGame->boardState->castlingFlags[0][BLACK_KINGS_SIDE] = 0;
			}
			else //white
			{
				if (move.initialPosition == 0x0000000000000080)
					chessGame->boardState->castlingFlags[0][WHITE_QUEENS_SIDE] = 0;
				else if (move.initialPosition == 0x0000000000000001)
					chessGame->boardState->castlingFlags[0][WHITE_KINGS_SIDE] = 0;
			}
		}

		//if user captured rook, update castling flags
		if (move.capturedPiece == BOARD_TYPE_ALL_ROOK_POSITIONS)
		{
			if (color == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
			{
				if (move.movedPosition == 0x0000000000000080)
					chessGame->boardState->castlingFlags[0][WHITE_QUEENS_SIDE] = 0;
				else if (move.movedPosition == 0x0000000000000001)
					chessGame->boardState->castlingFlags[0][WHITE_KINGS_SIDE] = 0;
			}

			else
			{
				if (move.initialPosition == 0x8000000000000000)
					chessGame->boardState->castlingFlags[0][BLACK_QUEENS_SIDE] = 0;
				else if (move.initialPosition == 0x0100000000000000)
					chessGame->boardState->castlingFlags[0][BLACK_KINGS_SIDE] = 0;
			}
		}

		if (move.pieceType == BOARD_TYPE_ALL_PAWN_POSITIONS
			&& (move.movedPosition & 0x00000000ff000000)
			&& (move.initialPosition & 0x000000000000ff00))

			chessGame->boardState->enpassantFlags[0] = move.movedPosition;
		else//if not, reset Enpassant flags
			chessGame->boardState->enpassantFlags[0] = 0x0000000000000000;


		updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, color, move.pieceType, move.castling, move.enpassant, move.capturedPiece, 0);

		move = generateMove(chessGame->boardState, !color, MAX_RECURSION_DEPTH);

		updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, !color, move.pieceType, move.castling, move.enpassant, move.capturedPiece, 0);

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
		sendMove[9] = '\0';

		fprintf(file, "Engine (produced): initial->%lu, moved->%lu\n", move.initialPosition, move.movedPosition);
		fprintf(file, "Engine (sent): %s\n", sendMove);
		printf("%s\n", sendMove);


	}

	else if (strcmp(cmd, "xboard") == 0)
	{
		printf("\n");

	}

	else if (strcmp(cmd, "protover 2") == 0)
	{
		printf("feature sigint=0 sigterm=0 usermove=0 time=0 done=1\n");
	}

	else if (strcmp(cmd, "new") == 0)
	{
		initChessGame(chessGame);
		chessGame->running = 1;
	}

	else if (strcmp(cmd, "white") == 0)
	{
		setEngineColor(chessGame->slothChessEngine, 0);
	}

	else if (strcmp(cmd, "black") == 0)
	{
		setEngineColor(chessGame->slothChessEngine, 1);
	}



	fflush(stdout);
}



void playChessWithXboard(ChessGame* chessGame)
{
    const int BUFFER_SIZE = 512;
    char buffer[512];
	setbuf(stdin, NULL);
    
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

