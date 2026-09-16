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

// COMMENT OUT WHILST NOT DEBUGGING. VIOLATES MVC.
// MIRRORED VERTICALLY. (H8 = A1).
void showBoard(const Board &_board) {
    for(int i = Constants::NO_TILES - 1; i >= 0; i--) {
        uint16_t index = _board.indexMap[i];

        if(index == 0xFFFF) {
            cout << " - ";
        }
        else {
            PieceType type = retType(_board.pieces[index]);
            bool is_white = isWhite(_board.pieces[index]);
            bool is_playing = inPlay(_board.pieces[index]);

            if(is_playing == false) {
                cout << " - ";
            }
            if(type == ROOK) {
                if(is_white) {
                    cout << " R ";
                }
                else cout << " r ";
            }
            if(type == KNIGHT) {
                if(is_white) {
                    cout << " N ";
                }
                else cout << " n ";
            }
            if(type == PAWN) {
                if(is_white) {
                    cout << " P ";
                }
                else cout << " p ";
            }
            if(type == BISHOP) {
                if(is_white) {
                    cout << " B ";
                }
                else cout << " b ";
            }
            if(type == KING) {
                if(is_white) {
                    cout << " K ";
                }
                else cout << " k ";
            }
            if(type == QUEEN) {
                if(is_white) {
                    cout << " Q ";
                }
                else cout << " q ";
            }
        }
        if(i % 8 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}
