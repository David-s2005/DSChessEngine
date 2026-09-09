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
                        std::exit(EXIT_FAILURE);
                        // This shouldn't occur at all. If so,
                        // i've done something catastrophic.
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
