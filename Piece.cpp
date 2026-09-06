#include "Piece.h"

// 16 bits = 0000000000000000

bool inPlay(Piece _piece) {
    uint16_t inPlayMask = 0x0000000000000001;
    return _piece & inPlayMask;
}

bool isWhite(Piece _piece) {
    uint16_t blackMask = 0x0000000000000010;
    return !((_piece & blackMask) << 1);
}
