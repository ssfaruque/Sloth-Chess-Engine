
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * A Chess AI that can play chess                                               *
 ********************************************************************************/


/**
 *  @file move.h
 *  @author Sahil Faruque
 *  @author Delvin Huynh
 *  @author Henry Tran
 *  @date 21 Jan 2018
 *  @brief All necessary logic for move generation.
 */



#ifndef move_h
#define move_h

#include "bitboard.h"


#define MAX_RECURSION_DEPTH 3


/**
 @brief The color of the player (white or black).
 */
enum PlayerType
{
  PLAYER_TYPE_WHITE,
  PLAYER_TYPE_BLACK
};


/**
 @brief Type of chess piece (pawn, rook, knight, bishop, queen, or king).
 */
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


/**
 @brief Move contains two Bitboards (one for initial position and one for moved position).
 */
typedef struct
{
  Bitboard initialPosition; /**< The Bitboard containing all 0s except for a 1 in the spot of
                                 the initial position of the specific piece */
  Bitboard movedPosition; /**< The Bitboard containing all 0s except for a 1 in the spot of
                               the new moved position of the specific piece */
  int boardEval;
} Move;


/**
 @brief Moves contains two arrays of type Move (One for quiet moves and another for capture moves).
 */
typedef struct
{
  Move* quietMoves; /**< The array containing all of the quiet moves which are moves that simply
                         move one piece from one square to another square without interrupting
                         another piece (the player does not capture an opponent's piece) */
  Move* captureMoves; /**< The array containing all of the capture moves which are moves that
                           move one piece from one square to another square and captures an opponent's
                           piece (the piece that is captured is the opponent's piece that has the 
                           same initial position as the player's moved position) */
  int numQuietMoves;
  int numCaptureMoves;
} Moves;



/**
 *  @brief Initializes the member variables of Moves to all NULL.
 *  @param moves The Moves struct we want to initialize
 *  @return void
 */
void initMoves(Moves* moves);


/**
 *  @brief Cleans up all of the resources used by the Moves struct.
 *  @param moves The Moves struct we want to clean up
 *  @return void
 */
void cleanUpMoves(Moves* moves);



/**
 *  @brief Updates the state of all Bitboards necessary after a move by the computer.
 *          has been performed
 *  @param boardState The BoardState that we want to update
 *  @param initialPiece The Bitboard that only contains the piece we want to move that has
 *          not moved yet
 *  @param movedPiece The Bitboard that only contains the piece that has moved
 *  @param colorType The color of the piece that we want to move
 *  @param pieceType The type of piece that we want to move
 *  @return void
 */
void updateBoardState(BoardState* boardState,
                      Bitboard initialPiece,
                      Bitboard movedPiece,
                      enum BitboardType colorType,
                      enum BitboardType pieceType);





/*********************************** MOVE GENERATION BY TYPE ***********************************/


/**
 *  @brief Generates a move recursively in the tree in accordance to DFS.
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @return The move that is going to be made
 */
Move generateMove(BoardState* boardState,
                  enum BitboardType colorType,
                  int recurseDepth);


/**
 *  @brief Generates a move for a pawn of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
Moves* generatePawnMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType);


/**
 *  @brief Generates a move for a rook of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
Moves* generateRookMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType);



/**
 *  @brief Generates a move for a knight of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
Moves* generateKnightMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType);


/**
 *  @brief Generates a move for a bishop of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
Moves* generateBishopMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType);


/**
 *  @brief Generates a move for a queen of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
Moves* generateQueenMoves(BoardState* boardState,
                        Bitboard isolatedPiece,
                        enum BitboardType colorType);


/**
 *  @brief Generates a move for a king of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
Moves* generateKingMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType);


/***********************************************************************************************/



/************************************ SLIDE MOVE GENERATION ************************************/


/**
 *  @brief Generates a move that slides left for a piece.
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateSlideLeftMove(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           enum BitboardType pieceType,
                           int offset);



/**
 *  @brief Generates a move that slides right for a piece.
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateSlideRightMove(Bitboard initialPosition,
                            BoardState* boardState,
                            enum BitboardType colorType,
                            enum BitboardType pieceType,
                            int offset);




/**
 *  @brief Generates a move that slides up for a piece.
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateSlideUpMove(Bitboard initialPosition,
                         BoardState* boardState,
                         enum BitboardType colorType,
                         enum BitboardType pieceType,
                         int offset);




/**
 *  @brief Generates a move that slides down for a piece.
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateSlideDownMove(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           enum BitboardType pieceType,
                           int offset);

/***********************************************************************************************/



/************************************ DIAGONAL MOVE GENERATION ************************************/


/**
 *  @brief Generates a move that moves diagonally (up, right).
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateDiagonalUpRightMove(Bitboard initialPosition,
                                   BoardState* boardState,
                                   enum BitboardType colorType,
                                   enum BitboardType pieceType,
                                   int offset);



/**
 *  @brief Generates a move that moves diagonally (up, left).
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateDiagonalUpLeftMove(Bitboard initialPosition,
                                  BoardState* boardState,
                                  enum BitboardType colorType,
                                  enum BitboardType pieceType,
                                  int offset);



/**
 *  @brief Generates a move that moves diagonally (down, right).
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateDiagonalDownRightMove(Bitboard initialPosition,
                                     BoardState* boardState,
                                     enum BitboardType colorType,
                                     enum BitboardType pieceType,
                                     int offset);



/**
 *  @brief Generates a move that moves diagonally (down, left).
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateDiagonalDownLeftMove(Bitboard initialPosition,
                                    BoardState* boardState,
                                    enum BitboardType colorType,
                                    enum BitboardType pieceType,
                                    int offset);


/**************************************************************************************************/



/*************************************** L MOVE GENERATION ****************************************/


/**
 *  @brief Generates a move that moves in an L shape manner (e.g. up 2, left 1 or right 2, down 1).
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateLMove(Bitboard initialPosition,
                    BoardState* boardState,
                    enum BitboardType colorType,
                    enum BitboardType pieceType,
                    int offset);


/**
 *  @brief Generates a move that moves twice up and once to the right
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateUpUpRight(Bitboard initialPosition,
                       BoardState* boardState,
                       enum BitboardType colorType,
                       enum BitboardType pieceType);


/**
 *  @brief Generates a move that moves once up and twice to the right
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateUpRightRight(Bitboard initialPosition,
                          BoardState* boardState,
                          enum BitboardType colorType,
                          enum BitboardType pieceType);



/**
 *  @brief Generates a move that moves once down and twice to the right
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateDownRightRight(Bitboard initialPosition,
                            BoardState* boardState,
                            enum BitboardType colorType,
                            enum BitboardType pieceType);


/**
 *  @brief Generates a move that moves twice down and once to the right
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateDownDownRight(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           enum BitboardType pieceType);



/**
 *  @brief Generates a move that moves twice down and once to the left
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateDownDownLeft(Bitboard initialPosition,
                          BoardState* boardState,
                          enum BitboardType colorType,
                          enum BitboardType pieceType);



/**
 *  @brief Generates a move that moves once down and twice to the left
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateDownLeftLeft(Bitboard initialPosition,
                          BoardState* boardState,
                          enum BitboardType colorType,
                          enum BitboardType pieceType);



/**
 *  @brief Generates a move that moves once up and twice to the left
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateUpLeftLeft(Bitboard initialPosition,
                        BoardState* boardState,
                        enum BitboardType colorType,
                        enum BitboardType pieceType);


/**
 *  @brief Generates a move that moves twice up and once to the left
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param pieceType The piece that we are interested in
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateUpUpLeft(Bitboard initialPosition,
                      BoardState* boardState,
                      enum BitboardType colorType,
                      enum BitboardType pieceType);

/**************************************************************************************************/



/**
 *  @brief Finds which column the piece is located in
 *  @param initialPosition
 *  @return The column that the isolated piece is located in
 */
int findCol(Bitboard initialPosition);



int isKingInCheck(BoardState* boardState, enum BitboardType colorType);



#endif /* move_h */
