#include "Board.h"

#include <cstring>

void updateLookupMap(Board &_board) {
    memset(_board.indexMap, -1, sizeof(_board.indexMap)); // init all values to 0xFFFF

    for(uint16_t i = 0; i < Constants::NO_PIECES; i++) {
        if (!inPlay(_board.pieces[i])) continue; // Skip captured/off-board pieces.

        uint16_t index = retPosition(_board.pieces[i]);
        _board.indexMap[index] = i;
    }
}
