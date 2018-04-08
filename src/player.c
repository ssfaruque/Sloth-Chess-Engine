//
//  player.c
//  ChessEngine
//
//  Created by Sahil Faruque on 2/22/18.
//  Copyright © 2018 Sahil Faruque. All rights reserved.
//

#include "player.h"

#include "slothChessEngine.h"

#include <stdint.h>
#include <string.h>

static int XBOARD = 0;

char board[8][8];



void updateInternalBoard(BoardState* boardState)
{
  int row;
  int col;
  int boardNum;


  for(row = 0; row < 8; ++row)
  {
    for(col = 0; col < 8; ++col)
    {
      board[row][col] = '.';
    }
  }

  for(boardNum = 2; boardNum < 8; ++boardNum)
  {
    Bitboard whitePieces = boardState->boards[boardNum] & boardState->boards[BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS];
    Bitboard blackPieces = boardState->boards[boardNum] & boardState->boards[BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS];

    uint64_t i = 1;
    unsigned int pos = 0;

    while (pos < 64)
    {
      if(i & whitePieces)
      {
        row = 7 - pos / 8;
        col = 7 - pos % 8;
        board[row][col] = getSymbol(BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, boardNum);
      }

      // Unset current bit and set the next bit in 'i'
      i = i << 1;

      // increment position
      ++pos;

    }

    i = 1;
    pos = 0;


    while (pos < 64)
    {

      if(i & blackPieces)
      {
        row = 7 - pos / 8;
        col = 7 - pos % 8;
        board[row][col] = getSymbol(BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, boardNum);
      }

      // Unset current bit and set the next bit in 'i'
      i = i << 1;

      // increment position
      ++pos;

    }

  }
}


void generateFEN(SlothChessEngine* engine)
{

  updateInternalBoard(engine->boardState);

  int row;
  int col;
  int index = 0;
  int numEmpty = 0;

  for(row = 0; row < BOARD_LENGTH; ++row)
  {
    for(col = 0; col < BOARD_LENGTH; ++col)
    {
      if(board[row][col] != '.')
      {
        if(numEmpty)
        {
          engine->FEN[index++] = '0' + numEmpty;
          numEmpty = 0;
        }

        engine->FEN[index++] = board[row][col];
      }

      else
        ++numEmpty;
    }

    if(numEmpty)
    {
      engine->FEN[index++] = '0' + numEmpty;
      numEmpty = 0;
    }

    if(row != BOARD_LENGTH - 1)
      engine->FEN[index++] = '/';

  }

  engine->FEN[index++] = ' ';
  engine->FEN[index++] = 'w';
  engine->FEN[index++] = ' ';

  if(engine->boardState->castlingFlags[0][WHITE_KINGS_SIDE])
    engine->FEN[index++] = 'K';

  if(engine->boardState->castlingFlags[0][WHITE_QUEENS_SIDE])
    engine->FEN[index++] = 'Q';

  if(engine->boardState->castlingFlags[0][BLACK_KINGS_SIDE])
    engine->FEN[index++] = 'k';

  if(engine->boardState->castlingFlags[0][BLACK_QUEENS_SIDE])
    engine->FEN[index++] = 'q';

  if(engine->FEN[index -1] != 'K' && engine->FEN[index -1] != 'Q' &&
     engine->FEN[index -1] != 'k' && engine->FEN[index -1] != 'q')
      engine->FEN[index++] = '-';


  engine->FEN[index] = '\0';
}





void setBoardWithFEN(SlothChessEngine* engine, char* FEN)
{
  int index = 0;
  int space = 0;

  while(FEN[index] != ' ')
  {
    if(FEN[index] != '/')
    {
      int row = space / BOARD_LENGTH;
      int col = space % BOARD_LENGTH;

      char c = FEN[index++];

      if(c > '0' && c <= '8')
      {
        int numEmptySpaces = c - '0';

        int i;

        for(i = 0; i < numEmptySpaces; ++i)
        {
          board[row][col] = '.';
          ++space;
          row = space / BOARD_LENGTH;
          col = space % BOARD_LENGTH;
        }
      }

      else
      {
        board[row][col] = c;
        ++space;
      }

    }

    else
      ++index;
  }
}


void setBoardStateWithFEN(SlothChessEngine* engine, char* FEN)
{
  setBoardWithFEN(engine, FEN);

  char fen[100];
  strcpy(fen, FEN);


  /* set castling flags, if any */
  char* ptr = strtok(fen, " ");
  ptr = strtok(NULL, " ");
  ptr = strtok(NULL, " ");

  if(*ptr == '-')
  {
    engine->boardState->castlingFlags[0][WHITE_KINGS_SIDE]  =
    engine->boardState->castlingFlags[0][WHITE_QUEENS_SIDE] =
    engine->boardState->castlingFlags[0][BLACK_KINGS_SIDE]  =
    engine->boardState->castlingFlags[0][BLACK_QUEENS_SIDE] = 0;
  }

  else
  {
    while(*ptr == 'K' || *ptr == 'Q' || *ptr == 'k' || *ptr == 'q')
    {
      if(*ptr == 'K')
      {
        engine->boardState->castlingFlags[0][WHITE_KINGS_SIDE]  = 1;
      }

      else if(*ptr == 'Q')
      {
        engine->boardState->castlingFlags[0][WHITE_QUEENS_SIDE] = 1;
      }

      else if(*ptr == 'k')
      {
        engine->boardState->castlingFlags[0][BLACK_KINGS_SIDE] = 1;
      }

      else if(*ptr == 'q')
      {
        engine->boardState->castlingFlags[0][BLACK_QUEENS_SIDE] = 1;
      }

      ++ptr;
    }
  }




  int row,col;

  int i;

  Bitboard pawns   = 0;
  Bitboard rooks   = 0;
  Bitboard knights = 0;
  Bitboard bishops = 0;
  Bitboard queens  = 0;
  Bitboard kings   = 0;

  Bitboard whites = 0;
  Bitboard blacks = 0;

  // loop to get all pawns
  for(i = BOARD_LENGTH * BOARD_LENGTH - 1; i >= 0; --i)
  {
    int row = i / BOARD_LENGTH;
    int col = i % BOARD_LENGTH;

    char piece = board[row][col];

    if(piece == 'p' || piece == 'P')
    {
      Bitboard p = ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
      pawns |= p;


      if(piece == 'P')
        whites |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);

      else
        blacks |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
    }
  }



  // loop to get all rooks
  for(i = BOARD_LENGTH * BOARD_LENGTH - 1; i >= 0; --i)
  {
    int row = i / BOARD_LENGTH;
    int col = i % BOARD_LENGTH;

    char piece = board[row][col];

    if(piece == 'r' || piece == 'R')
    {
      Bitboard p = ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
      rooks |= p;


      if(piece == 'R')
        whites |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);

      else
        blacks |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
    }
  }


  // loop to get all knights
  for(i = BOARD_LENGTH * BOARD_LENGTH - 1; i >= 0; --i)
  {
    int row = i / BOARD_LENGTH;
    int col = i % BOARD_LENGTH;

    char piece = board[row][col];

    if(piece == 'n' || piece == 'N')
    {
      Bitboard p = ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
      knights |= p;


      if(piece == 'N')
        whites |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);

      else
        blacks |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
    }
  }


  // loop to get all bishops
  for(i = BOARD_LENGTH * BOARD_LENGTH - 1; i >= 0; --i)
  {
    int row = i / BOARD_LENGTH;
    int col = i % BOARD_LENGTH;

    char piece = board[row][col];

    if(piece == 'b' || piece == 'B')
    {
      Bitboard p = ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
      bishops |= p;


      if(piece == 'B')
        whites |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);

      else
        blacks |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
    }
  }


  // loop to get all queens
  for(i = BOARD_LENGTH * BOARD_LENGTH - 1; i >= 0; --i)
  {
    int row = i / BOARD_LENGTH;
    int col = i % BOARD_LENGTH;

    char piece = board[row][col];

    if(piece == 'q' || piece == 'Q')
    {
      Bitboard p = ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
      queens |= p;


      if(piece == 'Q')
        whites |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);

      else
        blacks |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
    }
  }


  // loop to get all kings
  for(i = BOARD_LENGTH * BOARD_LENGTH - 1; i >= 0; --i)
  {
    int row = i / BOARD_LENGTH;
    int col = i % BOARD_LENGTH;

    char piece = board[row][col];

    if(piece == 'k' || piece == 'K')
    {
      Bitboard p = ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
      kings |= p;


      if(piece == 'K')
        whites |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);

      else
        blacks |= ((int64_t) 1) << ((BOARD_LENGTH * BOARD_LENGTH) - 1 - i);
    }
  }

  engine->boardState->boards[0] = whites;
  engine->boardState->boards[1] = blacks;
  engine->boardState->boards[2] = pawns;
  engine->boardState->boards[3] = rooks;
  engine->boardState->boards[4] = knights;
  engine->boardState->boards[5] = bishops;
  engine->boardState->boards[6] = queens;
  engine->boardState->boards[7] = kings;
}





char getSymbol(enum BitboardType color, enum BitboardType pieceType)
{
  char offset = 0;

  if(color == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
    offset = ' ';

  switch(pieceType)
  {
    case BOARD_TYPE_ALL_PAWN_POSITIONS:
      return 'P' + offset;

    case BOARD_TYPE_ALL_ROOK_POSITIONS:
      return 'R' + offset;

    case BOARD_TYPE_ALL_KNIGHT_POSITIONS:
      return 'N' + offset;

    case BOARD_TYPE_ALL_BISHOP_POSITIONS:
      return 'B' + offset;

    case BOARD_TYPE_ALL_QUEEN_POSITIONS:
      return 'Q' + offset;

    case BOARD_TYPE_ALL_KING_POSITIONS:
      return 'K' + offset;
  }

  return ' '; // ERROR
}






void printBoardGUI(BoardState* boardState)
{
  int row;
  int col;

  updateInternalBoard(boardState);

  for(row = 0; row < 8; ++row)
  {
    printf("%d | ", 8 - row);

    for(col = 0; col < 8; ++col)
    {
      printf("%c ", board[row][col]);
    }

    printf("\n");
  }

  printf("   ----------------\n");
  printf("    a b c d e f g h\n");
}


int getPieceType(Bitboard isolatedBoard, enum BitboardType color, BoardState* boardState)
{
  int pieceType;

  for(pieceType = 2; pieceType < 8; ++pieceType)
  {
    Bitboard pieces = boardState->boards[color] & boardState->boards[pieceType];

    if(pieces & isolatedBoard)
    {
      return pieceType;
    }
  }

  return 0;
}



void processXboardCmd(ChessGame* chessGame, const char* cmd, FILE* file)
{
  /* user entered a move */
  if(cmd[1] >= '1' && cmd[1] <= '8' || strcmp(cmd, "go") == 0)
  {
    int beforeCol = cmd[0] - 'a' + 1;
    int beforeRow = cmd[1] - '0';
    int afterCol = cmd[2] - 'a' + 1;
    int afterRow = cmd[3] - '0';

	Move move;
	Bitboard initialPiece = ((int64_t)1) << ((beforeRow  - 1) * 8 + (8 - beforeCol));
    	Bitboard movedPiece = ((int64_t)1) << ((afterRow  - 1) * 8 + (8 - afterCol));

	int color = chessGame->slothChessEngine->playerType;

    /* pawn promotion (NOT FINISHED)*/
    if(strlen(cmd) == 5)
    {
    	char pieceChar = cmd[4];
	int promotedPiece = -1;

	switch(promotedPiece)
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

	if(strcmp(cmd, "e1g1") == 0)		/* white king's side */
	{
    		move.initialPosition = initialPiece;
    		move.movedPosition = movedPiece;
    		move.castling = KINGS_SIDE;
    		move.enpassant = 0;
    		move.pieceType =   BOARD_TYPE_ALL_KING_POSITIONS;
    		move.capturedPiece = 0;
            chessGame->boardState->castlingFlags[0][WHITE_KINGS_SIDE] = 0;
		fprintf(file, "Xboard: Performed castling!\n");
	}

	else if(strcmp(cmd, "e1c1") == 0)	/* white queen's side */
	{
		move.initialPosition = initialPiece;
    		move.movedPosition = movedPiece;
    		move.castling = QUEENS_SIDE;
    		move.enpassant = 0;
    		move.pieceType =   BOARD_TYPE_ALL_KING_POSITIONS;
    		move.capturedPiece = 0;
            chessGame->boardState->castlingFlags[0][WHITE_QUEENS_SIDE] = 0;
		fprintf(file, "Xboard: Performed castling!\n");
	}

	else if(strcmp(cmd, "e8g8") == 0)	/* black king's side */
	{
		move.initialPosition = initialPiece;
    		move.movedPosition = movedPiece;
    		move.castling = KINGS_SIDE;
    		move.enpassant = 0;
    		move.pieceType =   BOARD_TYPE_ALL_KING_POSITIONS;
    		move.capturedPiece = 0;
            chessGame->boardState->castlingFlags[0][BLACK_KINGS_SIDE] = 0;
		fprintf(file, "Xboard: Performed castling!\n");
	}

	else if(strcmp(cmd, "e8c8") == 0)	/* black queen's side */
	{
		move.initialPosition = initialPiece;
    		move.movedPosition = movedPiece;
    		move.castling = QUEENS_SIDE;
    		move.enpassant = 0;
    		move.pieceType =   BOARD_TYPE_ALL_KING_POSITIONS;
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

			chessGame->boardState->enpassantFlags[0] =  move.movedPosition;
		else//if not, reset Enpassant flags
			chessGame->boardState->enpassantFlags[0] =  0x0000000000000000;


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
	fclose(file);
    printf("%s\n", sendMove);


  }

  else if(strcmp(cmd, "xboard") == 0)
  {
    printf("\n");

  }

  else if(strcmp(cmd, "protover 2") == 0)
  {
    printf("feature sigint=0 sigterm=0 usermove=0 time=0 done=1\n");
  }

  else if(strcmp(cmd, "new") == 0)
  {
    initChessGame(chessGame);
    chessGame->running = 1;
  }

  else if(strcmp(cmd, "white") == 0)
  {
    setEngineColor(chessGame->slothChessEngine, 0);
  }

  else if(strcmp(cmd, "black") == 0)
  {
    setEngineColor(chessGame->slothChessEngine, 1);
  }



  fflush(stdout);
}



void runXboard(ChessGame* chessGame)
{
  const int BUFFER_SIZE = 512;
  char buffer[512];

  //setbuf(stdout, NULL);
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




void playerPlayChess(ChessGame* chessGame)
{
  int i = 0;

  char moveString[10];

  int playerColor = BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS;
  int engineColor = BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS;

  //setBoardStateWithFEN(chessGame->slothChessEngine, "rnb1k1nr/pp3ppp/2p2q2/2bpp3/4P2P/2N2N2/PPPP1PPR/R1BQKB2");
if (XBOARD)
    runXboard(chessGame);



  printBoardGUI(chessGame->boardState);

  while(1)
  {
    printf("Turn white %d\n", chessGame->slothChessEngine->turn);

    generateFEN(chessGame->slothChessEngine);
    printf("%s\n", chessGame->slothChessEngine->FEN);
    printBoardGUI(chessGame->boardState);

    fgets(moveString, 10, stdin);
    uint64_t spaceNumInitial = ((moveString[1] - '1') * 8) + ('h' - moveString[0]);
    uint64_t spaceNumMoved = ((moveString[3] - '1') * 8) + ('h' - moveString[2]);

    uint64_t one = 1;

	Move playerMove = { 0,0,0,0,0,0,0 };
    playerMove.initialPosition = (one << spaceNumInitial);
    playerMove.movedPosition = (one << spaceNumMoved);



    playerMove.pieceType = getPieceType(playerMove.initialPosition, playerColor, chessGame->boardState);
    playerMove.capturedPiece = getPieceType(playerMove.movedPosition, engineColor, chessGame->boardState);

     if (playerMove.pieceType == BOARD_TYPE_ALL_KING_POSITIONS)
        {

                chessGame->boardState->castlingFlags[0][WHITE_QUEENS_SIDE] = 0;
                chessGame->boardState->castlingFlags[0][WHITE_KINGS_SIDE] = 0;

            if (playerMove.movedPosition == playerMove.initialPosition >> 2)
                playerMove.castling = KINGS_SIDE;

            else if (playerMove.movedPosition == playerMove.initialPosition << 2)
                playerMove.castling = KINGS_SIDE;
        }

        //check if rook to update flags

        if (playerMove.pieceType == BOARD_TYPE_ALL_ROOK_POSITIONS)
        {

                if (playerMove.initialPosition == 0x0000000000000080)
                    chessGame->boardState->castlingFlags[0][WHITE_QUEENS_SIDE] = 0;
                else if (playerMove.initialPosition == 0x0000000000000001)
                    chessGame->boardState->castlingFlags[0][WHITE_KINGS_SIDE] = 0;

        }

        //if user captured rook, update castling flags
        if (playerMove.capturedPiece == BOARD_TYPE_ALL_ROOK_POSITIONS)
        {
            if (playerColor == BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS)
            {
                if (playerMove.movedPosition == 0x0000000000000080)
                     chessGame->boardState->castlingFlags[0][WHITE_QUEENS_SIDE] = 0;
                else if (playerMove.movedPosition == 0x0000000000000001)
                    chessGame->boardState->castlingFlags[0][WHITE_KINGS_SIDE] = 0;
            }

            else
            {
                if (playerMove.initialPosition == 0x8000000000000000)
                    chessGame->boardState->castlingFlags[0][BLACK_QUEENS_SIDE] = 0;
                else if (playerMove.initialPosition == 0x0100000000000000)
                    chessGame->boardState->castlingFlags[0][BLACK_KINGS_SIDE] = 0;
            }
        }


	if (playerMove.pieceType == BOARD_TYPE_ALL_PAWN_POSITIONS
			&& (playerMove.movedPosition & 0x00000000ff000000)
			&& (playerMove.initialPosition & 0x000000000000ff00))

			chessGame->boardState->enpassantFlags[0] =  playerMove.movedPosition;
		else//if not, reset Enpassant flags
			chessGame->boardState->enpassantFlags[0] =  0x0000000000000000;




    printf("Piece type: %c\n", getSymbol(playerColor, playerMove.pieceType));
    printf("Captured piece type: %c\n", getSymbol(engineColor, playerMove.capturedPiece));


    updateBoardState(chessGame->boardState, playerMove.initialPosition, playerMove.movedPosition, playerColor,
					playerMove.pieceType, playerMove.castling, playerMove.enpassant
					, playerMove.capturedPiece, 0);


    generateFEN(chessGame->slothChessEngine);
    printf("%s\n", chessGame->slothChessEngine->FEN);
    printBoardGUI(chessGame->boardState);


    i++;
    chessGame->slothChessEngine->turn++;





    // ------------------------------------------


    Move move = {0, 0, 0, 0, 0, 0, 0};
    move = generateMove(chessGame->boardState, engineColor, MAX_RECURSION_DEPTH);


    if(isKingInCheck(chessGame->boardState, engineColor, 1))
    {
      if(move.initialPosition == 0)
      {
        printf("Checkmate, black loses\n");
        break;
      }
    }


    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, engineColor,
		move.pieceType, move.castling, move.enpassant, move.capturedPiece, 0);

    chessGame->slothChessEngine->turn++;

    printf("Turn black: %d\n", chessGame->slothChessEngine->turn);
    printf("Score = %d\n", eval(chessGame->boardState));
    printf("Piece type: %c\n", getSymbol(engineColor, move.pieceType));
    printf("Captured piece type: %c\n", getSymbol(playerColor, move.capturedPiece));

    // ---------------------------------------------


  }

}
