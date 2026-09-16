#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "Piece.h"
#include "Constants.h"

using lookupMap = uint16_t[Constants::NO_TILES];

enum CastlingRights : uint16_t {
    WHITE_KINGSIDE = 0, WHITE_QUEENSIDE = 1,
    BLACK_KINGSIDE = 2, BLACK_QUEENSIDE = 3
};

struct Board {
    Piece pieces[Constants::NO_PIECES];
    lookupMap indexMap;
    color moving;
    // 4 bit int that represents white & blacks kingside & queenside
    // castling rights. Check documentation for more info.
    CastlingRights CR;
    positionRF EnPassantTarget;
};

// Goes through the pieces within the _board structure, and appends
// each of the pieces indexes to the boards lookup map.
void updateLookupMap(Board &_board);

void showBoard(const Board &_board);

#endif // BOARD_H_INCLUDED
