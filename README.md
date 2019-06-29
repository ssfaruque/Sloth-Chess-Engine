# Sloth-Chess-Engine
Chess AI

## Changing Recursion Depth
Inside bitboard.h:

Make changes to: #define MAX_RECURSION_DEPTH desired_value

## Switching between Xboard or GUI chess play
Inside player.c:

static int XBOARD = 1;// Enable XBoard

static int XBOARD = 0;// Enable Console GUI
