#include "output.h"
#include "Constants.h"

// Takes a given board state and returns its equivalent FEN string.
// If you'd like to know more about FEN, check the documentation.
string RetFEN(const Board &_board, bool _white_turn) {
    int cons_empty_tiles = 0; // consecutive empty tiles.
    string str;
    // Count down, rank 8 -> rank 1. Within each rank, walk file A -> H.
    for(uint16_t rank = Constants::BOARD_SIDE_LEN; rank != 0; rank--) {
            cons_empty_tiles = 0;
        for(uint16_t file = 1; file != Constants::BOARD_SIDE_LEN + 1; file++) {
            positionRF pos = {static_cast<uint16_t>(rank - 1), static_cast<uint16_t>(file - 1)};
            uint16_t index = _board.indexMap[RFToIndex(pos)];

            if(index == Constants::SENTINEL) { // empty tile.
                cons_empty_tiles++;
            }
            else {
                // Write the total amount of consecutive encountered prior to this
                // occupied tile.
                if(cons_empty_tiles > 0) {
                    str += std::to_string(cons_empty_tiles);
                    cons_empty_tiles = 0;
                }

                PieceType type = retType(_board.pieces[index]);
                bool white = isWhite(_board.pieces[index]);

                switch(type) {
                    case PAWN:
                        if(white) str += 'P';
                        else str += 'p';
                        break;

                    case KNIGHT:
                        if(white) str += 'N';
                        else str += 'n';
                        break;

                    case BISHOP:
                        if(white) str += 'B';
                        else str += 'b';
                        break;

                    case ROOK:
                        if(white) str += 'R';
                        else str += 'r';
                        break;

                    case QUEEN:
                        if(white) str += 'Q';
                        else str += 'q';
                        break;

                    case KING:
                        if(white) str += 'K';
                        else str += 'k';
                        break;

                    case NONE:
                        // Unreachable: the loop above already skips empty
                        // tiles via the SENTINEL check.
                        break;
                }
            }
        }
        // Append empty row given no valid peices were seen. Needed because the condition
        // within the loop (the one that does the same thing) only executes if a actual
        // piece is seen after some empty tiles were seen.
        if(cons_empty_tiles > 0) {
            str += std::to_string(cons_empty_tiles);
            cons_empty_tiles = 0;
        }

        // condition to prevent last row from being given a slash, which is invalid FEN.
        if (rank != 1) str += '/';
    }

    // whites turn?
    if(_white_turn) str += " w ";
    else str += (" b ");

    str += "KQkq "; // castling rights. invalid stump, change later.
    str += "- "; // en passant target square. Invalid stump.
    str += "0 "; // Total half-moves. Invalid stump.
    str += "1"; // Full-moves. Invalid stump.

    return str;
}

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
