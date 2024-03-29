#ifndef move_h
#define move_h

#include "bitboard.h"



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

enum MoveType
{
    MOVE_TYPE_QUIET,
    MOVE_TYPE_CAPTURE,
    MOVE_TYPE_PASSANT,
    MOVE_TYPE_CASTLE
};


enum CastlingType
{
    NO_CASTLING,
    KINGS_SIDE,
    QUEENS_SIDE
};

enum EnpassantType
{
    NO_ENPASSANT,
    ENPASSANT_UPLEFT,
    ENPASSANT_UPRIGHT,
    ENPASSANT_DOWNLEFT,
    ENPASSANT_DOWNRIGHT
};

enum CastlingFlags
{
    WHITE_KINGS_SIDE,
    WHITE_QUEENS_SIDE,
    BLACK_KINGS_SIDE,
    BLACK_QUEENS_SIDE
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
    int pieceType;
    int capturedPiece;
    int boardEval;
    int enpassant;
    int castling;
} Move;





#define NUM_PIECES 6
#define MAX_NUM_MOVES 40


typedef struct
{
    Move moves[NUM_PIECES][MAX_NUM_MOVES];
    
    union
    {
        int numMoves[NUM_PIECES];
        
        struct
        {
            int numPawnMoves;
            int numRookMoves;
            int numKnightMoves;
            int numBishopMoves;
            int numQueenMoves;
            int numKingMoves;
        };
    };
    
    
} Moves;





void updateFlagState(BoardState* boardState,
                     Bitboard initialPiece,
                     Bitboard movedPiece,
                     enum BitboardType colorType,
                     enum BitboardType pieceType,
                     enum MoveType moveType);

/**
 *  @brief Updates the state of all Bitboards necessary after a move by the computer.
 *          has been performed
 *  @param boardState The BoardState that we want to update
 *  @param initialPiece The Bitboard that only contains the piece we want to move that has
 *          not moved yet
 *  @param movedPiece The Bitboard that only contains the piece that has moved
 *  @param colorType The color of the piece that we want to move
 *  @param pieceType The type of piece that we want to move
 *  @param moveType The type of move: Quiet, Capture, En Passant, or Castling
 *  @param capturedPiece THe pieceType that was captured just before. Used for unmoving a capture
 *  @return int, representing the pieceType that was captured
 */
int updateBoardState(BoardState* boardState,
                     Bitboard initialPiece,
                     Bitboard movedPiece,
                     enum BitboardType colorType,
                     enum BitboardType pieceType,
                     enum CastlingType castling,
                     enum EnpassantType enpassant,
                     int capturedPiece,
                     int undo);





/*********************************** MOVE GENERATION BY TYPE ***********************************/

int findCapturedPiece(BoardState* boardState, Bitboard movedPosition, enum BitboardType colorType);

//int castlingCheck(enum BitboardType colorType, Bitboard movedPosition);

int isKingInCheck(BoardState* boardState, enum BitboardType colorType, int moveLevel);


int isRookinCheck(BoardState* boardstate, enum BitboardType colorType, enum CastlingType castling, int moveLevel);
/* Skeleton functions to fill out */


/* These functions generate all of the possible moves for a particular
 piece and place them inside the moves list */

void generateAllSlidingMoves(BoardState* boardState,
                             Bitboard isolatedPiece,
                             int pieceType,
                             enum BitboardType colorType,
                             Moves* moves, int moveLevel);


void generateAllDiagonalMoves(BoardState* boardState,
                              Bitboard isolatedPiece,
                              int pieceType,
                              enum BitboardType colorType,
                              Moves* moves);


int maxi(BoardState* boardState, enum BitboardType colorType, int depth);
int mini(BoardState* boardState, enum BitboardType colorType, int depth);



int alphaBetaMax(BoardState* boardState, int alpha, int beta, enum BitboardType colorType, int depthleft);
int alphaBetaMin(BoardState* boardState, int alpha, int beta, enum BitboardType colorType, int depthleft);








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


void generateCoreMoves(BoardState* boardState,
                       enum BitboardType colorType,
                       Moves* moves, int moveLevel);

void generateAllMoves(BoardState* boardState,
                      enum BitboardType colorType,
                      Moves* moves, int moveLevel);




void generateAllPawnMoves(BoardState* boardState,
                          enum BitboardType colorType,
                          Moves* moves, int moveLevel);




/**
 *  @brief Generates a move for a pawn of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
void generatePawnMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType,
                       Moves* moves, int moveLevel);



void generateAllRookMoves(BoardState* boardState,
                          enum BitboardType colorType,
                          Moves* moves);




/**
 *  @brief Generates a move for a rook of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
void generateRookMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType,
                       Moves* moves);



void generateAllKnightMoves(BoardState* boardState,
                            enum BitboardType colorType,
                            Moves* moves);


/**
 *  @brief Generates a move for a knight of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
void generateKnightMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType,
                         Moves* moves);



void generateAllBishopMoves(BoardState* boardState,
                            enum BitboardType colorType,
                            Moves* moves);




/**
 *  @brief Generates a move for a bishop of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
void generateBishopMoves(BoardState* boardState,
                         Bitboard isolatedPiece,
                         enum BitboardType colorType,
                         Moves* moves);



void generateAllQueenMoves(BoardState* boardState,
                           enum BitboardType colorType,
                           Moves* moves);


/**
 *  @brief Generates a move for a queen of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
void generateQueenMoves(BoardState* boardState,
                        Bitboard isolatedPiece,
                        enum BitboardType colorType,
                        Moves* moves);



void generateAllKingMoves(BoardState* boardState,
                          enum BitboardType colorType,
                          Moves* moves, int moveLevel);



/**
 *  @brief Generates a move for a king of a specific color.
 *         recursively in the tree in accordance to DFS
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @return The move that is going to be made
 */
void generateKingMoves(BoardState* boardState,
                       Bitboard isolatedPiece,
                       enum BitboardType colorType,
                       Moves* moves, int moveLevel);


/***********************************************************************************************/



/************************************ SLIDE MOVE GENERATION ************************************/


/**
 *  @brief Generates a move that slides left for a piece.
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateSlideLeftMove(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           int offset);



/**
 *  @brief Generates a move that slides right for a piece.
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateSlideRightMove(Bitboard initialPosition,
                            BoardState* boardState,
                            enum BitboardType colorType,
                            int offset);




/**
 *  @brief Generates a move that slides up for a piece.
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateSlideUpMove(Bitboard initialPosition,
                         BoardState* boardState,
                         enum BitboardType colorType,
                         int offset);




/**
 *  @brief Generates a move that slides down for a piece.
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateSlideDownMove(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType,
                           int offset);

/***********************************************************************************************/



/************************************ DIAGONAL MOVE GENERATION ************************************/


/**
 *  @brief Generates a move that moves diagonally (up, right).
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateDiagonalUpRightMove(Bitboard initialPosition,
                                 BoardState* boardState,
                                 enum BitboardType colorType,
                                 int offset);



/**
 *  @brief Generates a move that moves diagonally (up, left).
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateDiagonalUpLeftMove(Bitboard initialPosition,
                                BoardState* boardState,
                                enum BitboardType colorType,
                                int offset);



/**
 *  @brief Generates a move that moves diagonally (down, right).
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateDiagonalDownRightMove(Bitboard initialPosition,
                                   BoardState* boardState,
                                   enum BitboardType colorType,
                                   int offset);



/**
 *  @brief Generates a move that moves diagonally (down, left).
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @param offset How many squares we want to move the piece by
 *  @return The Move struct containing the information needed to make a move
 */
Move generateDiagonalDownLeftMove(Bitboard initialPosition,
                                  BoardState* boardState,
                                  enum BitboardType colorType,
                                  int offset);


/**************************************************************************************************/



/*************************************** L MOVE GENERATION ****************************************/



/**
 *  @brief Generates a move that moves twice up and once to the right
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateUpUpRight(Bitboard initialPosition,
                       BoardState* boardState,
                       enum BitboardType colorType
                       );


/**
 *  @brief Generates a move that moves once up and twice to the right
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateUpRightRight(Bitboard initialPosition,
                          BoardState* boardState,
                          enum BitboardType colorType
                          );



/**
 *  @brief Generates a move that moves once down and twice to the right
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateDownRightRight(Bitboard initialPosition,
                            BoardState* boardState,
                            enum BitboardType colorType
                            );


/**
 *  @brief Generates a move that moves twice down and once to the right
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateDownDownRight(Bitboard initialPosition,
                           BoardState* boardState,
                           enum BitboardType colorType
                           );



/**
 *  @brief Generates a move that moves twice down and once to the left
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateDownDownLeft(Bitboard initialPosition,
                          BoardState* boardState,
                          enum BitboardType colorType
                          );



/**
 *  @brief Generates a move that moves once down and twice to the left
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateDownLeftLeft(Bitboard initialPosition,
                          BoardState* boardState,
                          enum BitboardType colorType
                          );



/**
 *  @brief Generates a move that moves once up and twice to the left
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateUpLeftLeft(Bitboard initialPosition,
                        BoardState* boardState,
                        enum BitboardType colorType
                        );


/**
 *  @brief Generates a move that moves twice up and once to the left
 *  @param initialPosition The Bitboard containing only the initial position of a specfiic piece
 *  @param boardState The current state of the game
 *  @param colorType The color that the player is playing as
 
 *  @return The Move struct containing the information needed to make a move,
 *          movedPosition field will be 0 if cannot make this particular move
 */
Move generateUpUpLeft(Bitboard initialPosition,
                      BoardState* boardState,
                      enum BitboardType colorType
                      );

/**************************************************************************************************/


int findCol(Bitboard initialPosition);


int findRow(Bitboard initialPosition);



#endif /* move_h */
