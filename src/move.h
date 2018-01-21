

#ifndef move_h
#define move_h

#include "bitboard.h"

enum PlayerType
{
  PLAYER_TYPE_WHITE,
  PLAYER_TYPE_BLACK
};

//typedef enum BitboardType PlayerType;


enum PieceType
{
  PIECE_TYPE_PAWN,
  PIECE_TYPE_ROOK,
  PIECE_TYPE_KNIGHT,
  PIECE_TYPE_BISHOP,
  PIECE_TYPE_QUEEN,
  PIECE_TYPE_KING,
  NUM_PIECES
};


typedef BoardState Move;

typedef struct
{
  Move* quietMoves;
  Move* captureMoves;
} Moves;


typedef struct
{
  Moves* possibleMoves[NUM_PIECES];
} PossibleMoves;



void initMoves(Moves* moves);
void cleanUpMoves(Moves* moves);
void initPossibleMoves(PossibleMoves* possibleMoves);
void cleanUpPossibleMoves(PossibleMoves* possibleMoves);


void updateBoardState(BoardState* boardState, Bitboard initialPiece, Bitboard movedPiece,
                      enum BitboardType colorType, enum BitboardType pieceType);


PossibleMoves* generatePossibleMoves(BoardState* boardState, enum PlayerType playerType);

Moves* generatePawnMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateRookMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateKnightMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateBishopMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateQueenMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateKingMoves(BoardState* boardState, enum PlayerType playerType);


Moves* generateSlideMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateSlideLeftMoves(BoardState* boardState, enum BitboardType colorType,
                              enum BitboardType pieceType);
Move* generateSlideLeftMove(Bitboard isolatedPiece, BoardState* boardState, enum BitboardType colorType,
                             enum BitboardType pieceType, int offset);
Moves* generateSlideRightMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateSlideRightMove(BoardState* boardState, enum PlayerType playerType, int offset);
Moves* generateSlideUpMoves(BoardState* boardState, enum BitboardType colorType,
                            enum BitboardType pieceType);
Bitboard generateSlideUpMove(Bitboard isolatedPiece, BoardState* boardState, enum BitboardType colorType,
                           enum BitboardType pieceType, int offset);
Moves* generateSlideDownMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateSlideDownMove(BoardState* boardState, enum PlayerType playerType, int offset);


Moves* generateDiagonalMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateDiagonalUpRightMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateDiagonalUpRightMove(BoardState* boardState, enum PlayerType playerType, int offset);
Moves* generateDiagonalUpLeftMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateDiagonalUpLeftMove(BoardState* boardState, enum PlayerType playerType, int offset);
Moves* generateDiagonalDownRightMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateDiagonalDownRightMove(BoardState* boardState, enum PlayerType playerType, int offset);
Moves* generateDiagonalDownLeftMoves(BoardState* boardState, enum PlayerType playerType);
Moves* generateDiagonalDownLeftMove(BoardState* boardState, enum PlayerType playerType, int offset);

Moves* generateStepMoves(BoardState* boardState, enum PlayerType playerType);

Moves* generateLMoves(BoardState* boardState, enum PlayerType playerType);

#endif /* move_h */
