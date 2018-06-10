#ifndef eval_h
#define eval_h

#include "bitboard.h"


enum PieceWeight
{
    WEIGHT_PAWN,
    WEIGHT_ROOK,
    WEIGHT_KNIGHT,
    WEIGHT_BISHOP,
    WEIGHT_QUEEN,
    WEIGHT_KING
};


extern const int weights[];


/**
 *  @brief Determines how many pieces are on the bitboard
 *  @param bitboard The board that contains the pieces we are interested in
 *  @return The number of pieces that are on the Bitboard that was supplied
 */
int countNumPieces(Bitboard bitboard);



/**
 *  @brief Determines the material score contribution to the eval fnunction
 *  @param boardState The current state of the game
 *  @return The material evaluation of the current state of the game
 *          more positive means better for white and more negative
 *          means better for black
 */
int materialEval(BoardState* boardState);



/**
 *  @brief Determines the center control score contribution to the eval function
 *  @param boardState The current state of the game
 *  @return The evaluation of player center control,
 *          more positive means better for white and more negative
 *          means better for black
 */
int centerControlEval(BoardState* boardState);



int spaceScore(enum BitboardType colorType, int space, int pieceType);


int pieceSquareEval(BoardState* boardState);



int bishopPairEval(BoardState* boardState);

int kingSafety(BoardState* boardState);


int findKingZone(BoardState* boardState, enum BitboardType colortype);

/**
 *  @brief Finds a value based on the current state of the game
 *  @param boardState The BoardState that we want to evaluate
 *  @return An integer describing the state of the game,
 *          more positive means good for white and more negative
 *          means good for black
 */
int eval(BoardState* boardState);



#endif /* eval_h */
