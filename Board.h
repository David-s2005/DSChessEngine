#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "Piece.h"
#include "Constants.h"

using lookupMap = uint16_t[Constants::NO_TILES];

struct Board {
    Piece pieces[Constants::NO_PIECES];
    lookupMap indexMap;
    color moving;
    bool CastlingRights[4];
    positionRF EnPassantTarget;

    Board();
};

struct movement {
    positionRF start;
    positionRF end;
    PieceType promotionType;
};

// Goes through the pieces within the _board structure, and appends
// each of the pieces indexes to the boards lookup map.
void updateLookupMap(Board &_board);

void showBoard(const Board &_board);

#endif // BOARD_H_INCLUDED
