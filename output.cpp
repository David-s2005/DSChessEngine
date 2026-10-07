#include "output.h"
#include "Constants.h"

// FILE RANK to FILE RANK
// Example of a valid input:
// A2A4 <- White pawn double jump to A4
movement readMove(const string &_str) {
    positionRF start = {Constants::SENTINEL, Constants::SENTINEL};
    positionRF end = {Constants::SENTINEL, Constants::SENTINEL};
    PieceType promotionType = NONE;

    // String is not 4 chars.
    if(_str.length() < 4 || _str.length() > 5) return {start, end};

    // Read files.
    for(int i = 0; i < 2; i++) {
        char c = _str[2*i];

        // Check if the file is within A-H (uppercase) or a-h (lowercase).
        if((c >= 'A' && c <= 'H') ||
           (c >= 'a' && c <= 'h'))
       {
            // 'A'/'a' should map to file 0, 'B'/'b' to 1, and so on.
            uint16_t file = (c >= 'a') ? static_cast<uint16_t>(c - 'a')
                                        : static_cast<uint16_t>(c - 'A');

            if(2*i == 0) {
                start.file = file;
            }
            else end.file = file;
       }
       else return {start, end};
    }

    for(int i = 0; i < 2; i++) {
        // Selects chars 1 & 3 (ranks).
        char c = _str[2*i + 1];
        int val = c - '0';

        // Check if val is within valid bounds.
        if(val < 8 || val > 1) {
            if(2*i + 1 == 1) {
                start.rank = val - 1;
            }
            else end.rank = val - 1;
        }
        else return {start, end};
    }

    if(_str.length() == 5) {
        if(_str[4] == 'q') promotionType = QUEEN;
        if(_str[4] == 'r') promotionType = ROOK;
        if(_str[4] == 'b') promotionType = BISHOP;
        if(_str[4] == 'n') promotionType = KNIGHT;
    }

    return {start, end, promotionType};
}

// Converts a movement into UCI coordinate notation (e.g. "e2e4", or "e7e8q"
// for a promotion). The reverse of readMove.
string moveToStr(movement _move) {
    string str;

    str += static_cast<char>('a' + _move.start.file);
    str += static_cast<char>('1' + _move.start.rank);
    str += static_cast<char>('a' + _move.end.file);
    str += static_cast<char>('1' + _move.end.rank);

    switch(_move.promotionType) {
        case QUEEN:  str += 'q'; break;
        case ROOK:   str += 'r'; break;
        case BISHOP: str += 'b'; break;
        case KNIGHT: str += 'n'; break;
        default: break; // NONE - not a promotion, nothing to append.
    }

    return str;
}
