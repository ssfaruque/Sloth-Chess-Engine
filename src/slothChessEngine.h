
/********************************************************************************
 * Copyright (C) 2018 by Sahil Faruque, Delvin Huynh, Henry Tran                *
 *                                                                              *
 * Sloth Chess Engine                                                           *
 *                                                                              *
 * A Chess AI that can play chess                                               *
 ********************************************************************************/



/**
 *  @file slothChessEngine.h
 *  @author Sahil Faruque
 *  @author Delvin Huynh
 *  @author Henry Tran
 *  @date 21 Jan 2018
 *  @brief The chess engine that decides what moves to make when playing a game of chess.
 *  @bug No known bugs
 *
 */


#ifndef slothChessEngine_h
#define slothChessEngine_h

#include "bitboard.h"
#include "move.h"


/**
 *  @brief The chess engine that contains all necessary components to generate a move
 */
typedef struct
{
  BoardState* boardState;         /**< The current state of the game  */
  enum PlayerType playerType;     /**< The color that the chess engine is currently playing as  */
  int turn;
} SlothChessEngine;


/**
 *  @brief Initializes the chess engine with the appropriate
 *  parameters so that it can start playing
 *  @param engine The uninitialized engine
 *  @param boardState The starting state of the chess game
 *  @param playerType The color that the chess engine is going to play as
 *  @return void
 */
void initSlothChessEngine(SlothChessEngine* engine, BoardState* boardState, enum PlayerType playerType);


/**
 *  @brief Cleans up all of the resources allocated for the chess engine
 *  @param engine The chess engine that we want to clean up
 *  @return void
 */
void cleanUpSlothEngine(SlothChessEngine* engine);


#endif /* slothChessEngine_h */
