#include "Board.h"

#include <iostream>
using std::cout;
using std::endl;

Board::Board() {
    initPieceArr(this->pieces);
    this->CastlingRights[0] = false;
    this->CastlingRights[1] = false;
    this->CastlingRights[2] = false;
    this->CastlingRights[3] = false;
    this->EnPassantTarget = {Constants::SENTINEL, Constants::SENTINEL};
    this->moving = WHITE;
}

void updateLookupMap(Board &_board) {
    // init all squares to the empty sentinel.
    for(uint16_t i = 0; i < Constants::NO_TILES; i++) {
        _board.indexMap[i] = Constants::SENTINEL;
    }

    for(uint16_t i = 0; i < Constants::NO_PIECES; i++) {
        if (!inPlay(_board.pieces[i])) continue; // Skip captured/off-board pieces.

        uint16_t index = retPosition(_board.pieces[i]);
        _board.indexMap[index] = i;
    }
}

// COMMENT OUT WHILST NOT DEBUGGING. VIOLATES MVC.
// Rank 8 at the top, rank 1 at the bottom, file A to H left to right -
// standard orientation from white's perspective.
void showBoard(const Board &_board) {
    for(int rank = Constants::BOARD_SIDE_LEN - 1; rank >= 0; rank--) {
        for(int file = 0; file < Constants::BOARD_SIDE_LEN; file++) {
            uint16_t squareIndex = RFToIndex({static_cast<uint16_t>(rank), static_cast<uint16_t>(file)});
            uint16_t index = _board.indexMap[squareIndex];

            if(index == Constants::SENTINEL) {
                cout << " - ";
            }
            else {
                PieceType type = retType(_board.pieces[index]);
                bool is_white = isWhite(_board.pieces[index]);

                // indexMap only ever holds in-play pieces (updateLookupMap skips
                // captured ones), so no separate is_playing check is needed here.
                switch(type) {
                    case ROOK:   cout << (is_white ? " R " : " r "); break;
                    case KNIGHT: cout << (is_white ? " N " : " n "); break;
                    case PAWN:   cout << (is_white ? " P " : " p "); break;
                    case BISHOP: cout << (is_white ? " B " : " b "); break;
                    case KING:   cout << (is_white ? " K " : " k "); break;
                    case QUEEN:  cout << (is_white ? " Q " : " q "); break;
                    case NONE:   break;
                }
            }
        }
        cout << endl;
    }
    cout << endl;
}
