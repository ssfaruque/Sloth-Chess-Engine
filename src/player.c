//
//  player.c
//  ChessEngine
//
//  Created by Sahil Faruque on 2/22/18.
//  Copyright © 2018 Sahil Faruque. All rights reserved.
//

#include "player.h"

char board[8][8];







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
  
  
  while(1)
  {
    
    printf("Turn White %d\n", chessGame->slothChessEngine->turn);
    printBoardGUI(chessGame->boardState);
    
    fgets(moveString, 10, stdin);
    uint64_t spaceNumInitial = ((moveString[1] - '1') * 8) + ('h' - moveString[0]);
    uint64_t spaceNumMoved = ((moveString[3] - '1') * 8) + ('h' - moveString[2]);
    
    uint64_t one = 1;
    
    Move playerMove;
    playerMove.initialPosition = (one << spaceNumInitial);
    playerMove.movedPosition = (one << spaceNumMoved);
    
    
    
    playerMove.pieceType = getPieceType(playerMove.initialPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, chessGame->boardState);
    playerMove.capturedPiece = getPieceType(playerMove.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, chessGame->boardState);
    
    printf("pieceType = %d, capturedPiece = %d\n", playerMove.pieceType, playerMove.capturedPiece);
    
    
    updateBoardState(chessGame->boardState, playerMove.initialPosition, playerMove.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, playerMove.pieceType, 0, playerMove.capturedPiece, 0);
    
    printBoardGUI(chessGame->boardState);
    
    //printBoardState(chessGame->boardState);
    
    
    
    //Move move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, MAX_RECURSION_DEPTH);
    
    
    /*if(isKingInCheck(chessGame->boardState, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS))
     {
     if(move.initialPosition == 0)
     {
     printf("Checkmate, white loses\n");
     break;
     }
     }*/
    
    
    //updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, move.pieceType, 0, move.capturedPiece, 0);
    
    i++;
    chessGame->slothChessEngine->turn++;
    
    
    /*printf("Turn White %d\n", chessGame->slothChessEngine->turn);
     printf("Score = %d\n", move.boardEval);
     printf("Piece type: %c\n", getSymbol(BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, move.pieceType));
     printf("Captured piece type: %c\n", getSymbol(BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, move.capturedPiece));
     */
    
    //printBoardState(chessGame->boardState);
    
    
    
    
    Move move;
    move = generateMove(chessGame->boardState, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, MAX_RECURSION_DEPTH);
    
    
    if(isKingInCheck(chessGame->boardState, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS))
    {
      if(move.initialPosition == 0)
      {
        printf("Checkmate, black loses\n");
        break;
      }
    }
    
    
    /*printf("Bitboards for black move\n");
    printBitboard(move.initialPosition);
    printBitboard(move.movedPosition);
    */
    
    
    updateBoardState(chessGame->boardState, move.initialPosition, move.movedPosition, BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, move.pieceType, 0, move.capturedPiece, 0);
    
    chessGame->slothChessEngine->turn++;
    
    printf("Turn Black: %d\n", chessGame->slothChessEngine->turn);
    printf("Score = %d\n", move.boardEval);
    printf("Piece type: %c\n", getSymbol(BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS, move.pieceType));
    printf("Captured piece type: %c\n", getSymbol(BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS, move.capturedPiece));
    
    
    //printBoardState(chessGame->boardState);
    
    
    //printBoardGUI(chessGame->boardState);
    
  }
  
}









