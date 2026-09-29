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
        1, // Pawn
        3, // Knight
        3, // Bishop
        5, // Rook
        9, // Queen
        0  // King
    };

#endif // CONSTANTS_H_INCLUDED
