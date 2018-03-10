//
//  player.c
//  ChessEngine
//
//  Created by Sahil Faruque on 2/22/18.
//  Copyright © 2018 Sahil Faruque. All rights reserved.
//

#include "player.h"

#include "slothChessEngine.h"



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

  engine->FEN[index] = '\0';
}





void setBoardWithFEN(SlothChessEngine* engine, char* FEN)
{
  int index = 0;
  int space = 0;

  while(FEN[index] != '\0')
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




void playerPlayChess(ChessGame* chessGame)
{
  int i = 0;

  char moveString[10];

  int playerColor = BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS;
  int engineColor = BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS;

  setBoardStateWithFEN(chessGame->slothChessEngine, "rnb1k1nr/pp3ppp/2p2q2/2bpp3/4P2P/2N2N2/PPPP1PPR/R1BQKB2");

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

    Move playerMove;
    playerMove.initialPosition = (one << spaceNumInitial);
    playerMove.movedPosition = (one << spaceNumMoved);



    playerMove.pieceType = getPieceType(playerMove.initialPosition, playerColor, chessGame->boardState);
    playerMove.capturedPiece = getPieceType(playerMove.movedPosition, engineColor, chessGame->boardState);


    printf("Piece type: %c\n", getSymbol(playerColor, playerMove.pieceType));
    printf("Captured piece type: %c\n", getSymbol(engineColor, playerMove.capturedPiece));


    updateBoardState(chessGame->boardState, playerMove.initialPosition, playerMove.movedPosition, playerColor, playerMove.pieceType, 0, playerMove.capturedPiece, 0);


    generateFEN(chessGame->slothChessEngine);
    printf("%s\n", chessGame->slothChessEngine->FEN);
    printBoardGUI(chessGame->boardState);


    i++;
    chessGame->slothChessEngine->turn++;





    // ------------------------------------------



    Move move;
    move = generateMove(chessGame->boardState, engineColor, MAX_RECURSION_DEPTH);


    if(isKingInCheck(chessGame->boardState, engineColor, 1))
    {
      if(move.initialPosition == 0)
      {
        printf("Checkmate, black loses\n");
        break;
      }
    }


    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, engineColor, move.pieceType, 0, move.capturedPiece, 0);

    chessGame->slothChessEngine->turn++;

    printf("Turn black: %d\n", chessGame->slothChessEngine->turn);
    printf("Score = %d\n", eval(chessGame->boardState));
    printf("Piece type: %c\n", getSymbol(engineColor, move.pieceType));
    printf("Captured piece type: %c\n", getSymbol(playerColor, move.capturedPiece));

    // ---------------------------------------------


  }

}
