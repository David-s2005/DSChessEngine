#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <cstdint>

namespace Constants {
    // Board dimensions
    inline constexpr uint16_t NO_TILES = 64;
    inline constexpr uint16_t NO_PIECES = 32;
    inline constexpr uint16_t BOARD_SIDE_LEN = 8;

    // Bit arithmetic constants.
    inline constexpr uint16_t INPLAY_BIT = 0;
    inline constexpr uint16_t COLOR_BIT = 1;
    inline constexpr uint16_t TYPE_START = 2,  TYPE_LEN = 3;
    inline constexpr uint16_t POS_START = 5,  POS_LEN = 6;
    inline constexpr uint16_t ID_START = 11, ID_LEN = 4;
    inline constexpr uint16_t MOVED_BIT = 15;

    // Piece positioning constants.
    inline constexpr uint16_t WHITE_PAWN_START_RANK = 1;
    inline constexpr uint16_t BLACK_PAWN_START_RANK = 6;

    // My version of null. Used to denote a empty tile.
    inline constexpr uint16_t SENTINEL = 0xFFFF;
    inline constexpr int CHECKMATE_SCORE = -1000000;

    // misc
    // The maximum amount of tiles a single piece can attack.
    // (Queen on a almost empty board.)
    inline constexpr uint16_t MAX_ATTACKS = 27;

    // Default to 4 if not specified.
    inline uint16_t MAX_DEPTH = 4;
}

inline constexpr int materialValues[] = {
    0, // None
    100, // Pawn
    300, // Knight
    300, // Bishop
    500, // Rook
    900, // Queen
    0  // King
};

inline constexpr int pieceSquareTable[7][64] = {
    {0}, // NONE

    {80, 80, 80, 80, 80, 80, 80, 80,
     50, 50, 50, 50, 50, 50, 50, 50,
     40, 40, 40, 40, 40, 40, 40, 40,
     30, 30, 30, 30, 30, 30, 30, 30,
     25, 25, 25, 25, 25, 25, 25, 25,
     18, 18, 18, 18, 18, 18, 18, 18,
     10, 10, 10, 10, 10, 10, 10, 10,
     0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 }, // PAWN

    {-30, -10, -30, -30, -30, -30, -10, -30,
     -30, -25, -25, -25, -25, -25, -25, -30,
     -30, -25, -2,  -2,  -2,  -2,  -12, -30,
     -30, -2,  30,  30,  30,  30,  -2 , -30,
     -30, -2,  30,  30,  30,  30,  -2 , -30,
     -30, -25, -2,  -2,  -2,  -2,  -12, -30,
     -30, -25, -25, -25, -25, -25, -25, -30,
     -30, -10, -30, -30, -30, -30, -10, -30}, // KNIGHT

   {-20, -10, -10, -10, -10, -10, -10, -20
    -10,   0,   0,   0,   0,   0,   0, -10
    -10,   0,   5,  10,  10,   5,   0, -10
    -10,   5,   5,  10,  10,   5,   5, -10
    -10,   0,  10,  10,  10,  10,   0, -10
    -10,  10,  10,  10,  10,  10,  10, -10
    -10,   5,   0,   0,   0,   0,   5, -10
    -20, -10, -40, -10, -10, -40, -10, -20}, // BISHOP

   { 0,   0,   0,   0,   0,   0,   0,   0,
     5,  10,  10,  10,  10,  10,  10,   5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
    -5,   0,   0,   0,   0,   0,   0,  -5,
     0,   0,   0,   5,   5,   0,   0,   0 }, // ROOK

   { -20, -10, -10, -5, -5,-10, -10, -20,
     -10,  0,   0,   0,  0,  0,   0, -10,
     -10,  0,   5,   5,  5,  5,   0, -10,
      -5,  0,   5,   5,  5,  5,   0, -5,
       0,  0,   5,   5,  5,  5,   0,  0,
     -10,  5,   5,   5,  5,  5,   0, -10,
     -10,  0,   5,   0,  0,  0,   0, -10,
     -20, -10, -10, -5, -5, -10, -10,-20 }, // QUEEN

   { -30, -40, -40, -50, -50, -40, -40, -30,
     -30, -40, -40, -50, -50, -40, -40, -30,
     -30, -40, -40, -50, -50, -40, -40, -30,
     -30, -40, -40, -50, -50, -40, -40, -30,
     -20, -30, -30, -40, -40, -30, -30, -20,
     -10, -20, -20, -20, -20, -20, -20, -10,
      20,  20,   0,   0,   0,   0,  20,  20,
      20,  30,  10,   0,   0,  10,  30,  20 } // KING
};

#endif // CONSTANTS_H_INCLUDED
