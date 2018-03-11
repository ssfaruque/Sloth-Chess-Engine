
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * A Chess AI that can play chess                                               *
 ********************************************************************************/


/**
 *  @file bitboard.h
 *  @author Sahil Faruque
 *  @author Delvin Huynh
 *  @author Henry Tran
 *  @date 21 Jan 2018
 *  @brief Defines mini API for bitboard and boardstate data structures.
 *  @bug No known bugs
 *
 */



#ifndef bitboard_h
#define bitboard_h

#include <stdint.h>


/**
 *  @brief Length of a chess board
 */
#define BOARD_LENGTH 8
#define MAX_RECURSION_DEPTH 5

/**
 *  @brief Type of Bitboard
 *
 *  Each BitboardType constant corresponds to a specific index in the <br>
 *  array of Bitboards in the BoardState struct except for NUM_BITBOARDS
 */
enum BitboardType
{
  BOARD_TYPE_ALL_WHITE_PIECES_POSITIONS,
  BOARD_TYPE_ALL_BLACK_PIECES_POSITIONS,
  BOARD_TYPE_ALL_PAWN_POSITIONS,
  BOARD_TYPE_ALL_ROOK_POSITIONS,
  BOARD_TYPE_ALL_KNIGHT_POSITIONS,
  BOARD_TYPE_ALL_BISHOP_POSITIONS,
  BOARD_TYPE_ALL_QUEEN_POSITIONS,
  BOARD_TYPE_ALL_KING_POSITIONS,

  NUM_BITBOARDS
};


/**
 *  @brief Bitboard is a 64 bit int representing the 64 squares on a chess board
 *
 *  Each Bitboard describes a certain aspect of the entire game <br>
 *  For example, a Bitboard could represent all of the white pieces or all of the pawns
 */
typedef uint64_t Bitboard;



/**
 *  @brief BoardState contains an array of 8 Bitboards
 *
 *  BoardState contains 8 Bitboards <br>
 *  1 containing all of the white pieces <br>
 *  1 containing all of the black pieces <br>
 *  1 containing all of the pawn pieces <br>
 *  1 containing all of the rook pieces <br>
 *  1 containing all of the knight pieces <br>
 *  1 containing all of the bishop pieces <br>
 *  1 containing all of the queen pieces <br>
 *  1 containing all of the king pieces <br>
 */
typedef struct
{
	Bitboard boards[NUM_BITBOARDS];
	Bitboard flagState;
	int8_t castlingFlags[MAX_RECURSION_DEPTH + 1][4];
	Bitboard enpassantFlags[MAX_RECURSION_DEPTH + 1];
} BoardState;


/**
 *  @brief Creates a new Bitboard
 *  @param squares A 64 bit integer representing all 64 squares of the chess board
 *  @return A bitboard with the squares filled with 0s and 1s as specified by the input
 */
Bitboard createBitboard(uint64_t squares);


/**
 *  @brief Initializes a boardState to the starting representation of a new chess game
 *  @param boardState The BoardState we want to initialize
 *  @return void
 */
void initBoardState(BoardState* boardState);


/**
 *  @brief Copies all 8 Bitboards from one BoardState to another BoardState <br>
 *         Creates a deep copy
 *  @param dst The destination BoardState (Where we are copying to)
 *  @param src The source BoardState (Where we are copying from)
 *  @return void
 */
void copyBoardState(BoardState* dst, BoardState* src);


/**
 *  @brief Prints out the entire Bitboard in a chess board representation
 *  @param bitboard The Bitboard that we want to print
 *  @return void
 *
 *  @verbatim

    Example of printing out the Bitboard of all black pieces starting out

    8 |1|1|1|1|1|1|1|1|
    7 |1|1|1|1|1|1|1|1|
    6 |0|0|0|0|0|0|0|0|
    5 |0|0|0|0|0|0|0|0|
    4 |0|0|0|0|0|0|0|0|
    3 |0|0|0|0|0|0|0|0|
    2 |0|0|0|0|0|0|0|0|
    1 |0|0|0|0|0|0|0|0|
       a b c d e f g h

    @endverbatim
 *
 */
void printBitboard(Bitboard bitboard);


/**
 *  @brief Prints out each of the 8 Bitboards in the BoardState struct
 *   with their corresponding titles
 *  @param boardState The BoardState that we want to print out
 *  @return void
    @verbatim

   Example of printing out the entire BoardState at the start of the game

   --- Printing out board state ---

   All White Pieces
   8 |0|0|0|0|0|0|0|0|
   7 |0|0|0|0|0|0|0|0|
   6 |0|0|0|0|0|0|0|0|
   5 |0|0|0|0|0|0|0|0|
   4 |0|0|0|0|0|0|0|0|
   3 |0|0|0|0|0|0|0|0|
   2 |1|1|1|1|1|1|1|1|
   1 |1|1|1|1|1|1|1|1|
      a b c d e f g h

   All Black Pieces
   8 |1|1|1|1|1|1|1|1|
   7 |1|1|1|1|1|1|1|1|
   6 |0|0|0|0|0|0|0|0|
   5 |0|0|0|0|0|0|0|0|
   4 |0|0|0|0|0|0|0|0|
   3 |0|0|0|0|0|0|0|0|
   2 |0|0|0|0|0|0|0|0|
   1 |0|0|0|0|0|0|0|0|
      a b c d e f g h

   All Pawn Positions
   8 |0|0|0|0|0|0|0|0|
   7 |1|1|1|1|1|1|1|1|
   6 |0|0|0|0|0|0|0|0|
   5 |0|0|0|0|0|0|0|0|
   4 |0|0|0|0|0|0|0|0|
   3 |0|0|0|0|0|0|0|0|
   2 |1|1|1|1|1|1|1|1|
   1 |0|0|0|0|0|0|0|0|
      a b c d e f g h

   All Rook Positions
   8 |1|0|0|0|0|0|0|1|
   7 |0|0|0|0|0|0|0|0|
   6 |0|0|0|0|0|0|0|0|
   5 |0|0|0|0|0|0|0|0|
   4 |0|0|0|0|0|0|0|0|
   3 |0|0|0|0|0|0|0|0|
   2 |0|0|0|0|0|0|0|0|
   1 |1|0|0|0|0|0|0|1|
      a b c d e f g h

   All Knight Positions
   8 |0|1|0|0|0|0|1|0|
   7 |0|0|0|0|0|0|0|0|
   6 |0|0|0|0|0|0|0|0|
   5 |0|0|0|0|0|0|0|0|
   4 |0|0|0|0|0|0|0|0|
   3 |0|0|0|0|0|0|0|0|
   2 |0|0|0|0|0|0|0|0|
   1 |0|1|0|0|0|0|1|0|
      a b c d e f g h

   All Bishop Positions
   8 |0|0|1|0|0|1|0|0|
   7 |0|0|0|0|0|0|0|0|
   6 |0|0|0|0|0|0|0|0|
   5 |0|0|0|0|0|0|0|0|
   4 |0|0|0|0|0|0|0|0|
   3 |0|0|0|0|0|0|0|0|
   2 |0|0|0|0|0|0|0|0|
   1 |0|0|1|0|0|1|0|0|
      a b c d e f g h

   All Queen Positions
   8 |0|0|0|1|0|0|0|0|
   7 |0|0|0|0|0|0|0|0|
   6 |0|0|0|0|0|0|0|0|
   5 |0|0|0|0|0|0|0|0|
   4 |0|0|0|0|0|0|0|0|
   3 |0|0|0|0|0|0|0|0|
   2 |0|0|0|0|0|0|0|0|
   1 |0|0|0|1|0|0|0|0|
      a b c d e f g h

   All King Positions
   8 |0|0|0|0|1|0|0|0|
   7 |0|0|0|0|0|0|0|0|
   6 |0|0|0|0|0|0|0|0|
   5 |0|0|0|0|0|0|0|0|
   4 |0|0|0|0|0|0|0|0|
   3 |0|0|0|0|0|0|0|0|
   2 |0|0|0|0|0|0|0|0|
   1 |0|0|0|0|1|0|0|0|
      a b c d e f g h

   --------------------------------
    @endverbatim
 */
void printBoardState(BoardState* boardState);


#endif /* bitboard_h */


