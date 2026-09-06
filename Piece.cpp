#include "Piece.h"
#include "Bitwise_Logic.h"

// Returns a boolean based on where the 1st bit is 1 or 0.
bool inPlay(const Piece _piece) {
    uint16_t inPlayMask = 0b1;
    return _piece & inPlayMask;
}

// Sets the is playing bit to the value passed (_is_playing).
void setPlay(Piece &_piece, bool _is_playing) {
    setSequence(_piece, 0, 1, _is_playing);
}

// Checks if the color bit (bit 1) is white or not. 1 = white.
bool isWhite(const Piece _piece) {
    uint16_t whiteMask = 0b10;
    return _piece & whiteMask;
}

// Sets the white flag to the value passed (_is_white).
void setWhite(Piece &_piece, bool _is_white) {
    setSequence(_piece, 1, 1, _is_white);
}

// Returns the 6 bits (5-10) of the piece 16 bit sequence that represent
// the pieces position (0-63, covering all squares of an 8x8 board).
uint16_t retPosition(const Piece _piece) {
    return retSequence(_piece, 5, 6);
}

// Returns the position of a given piece in rank-file form.
positionRF retPositionRF(const Piece _piece) {
    uint16_t pos = retPosition(_piece);
    positionRF ret = {static_cast<uint16_t>(pos / 8),
                      static_cast<uint16_t>(pos % 8)};
    return ret;
}

// Sets the position of the provided piece by rank and file. Decodes RF
// into an index the 16 bit piece number can accept.
void setPositionRF(const uint16_t _rank, const uint16_t _file, Piece &_piece) {
    uint16_t index = static_cast<uint16_t>((_rank * 8) + _file);
    setSequence(_piece, 5, 6, index);
}

// Sets position by index, remember that position is a 0-63 value.
void setPositionI(const uint16_t _index, Piece &_piece) {
    setSequence(_piece, 5, 6, _index);
}

// Returns the 3 bits (2-4) that represent the piece's type.
PieceType retType(const Piece _piece) {
    return static_cast<PieceType>(retSequence(_piece, 2, 3));
}

// Sets the 3 bits (2-4) that represent the piece's type.
void setType(const PieceType _type, Piece &_piece) {
    setSequence(_piece, 2, 3, static_cast<uint16_t>(_type));
}

// Returns the 4 bits (11-14) used alongside the color flag to
// uniquely identify a piece.
uint16_t retID(const Piece _piece) {
    return retSequence(_piece, 11, 4);
}

// Sets the 4 bits (11-14) used alongside the color flag to
// uniquely identify a piece.
void setID(const uint16_t _id, Piece &_piece) {
    setSequence(_piece, 11, 4, _id);
}

// Checks the has-moved flag (bit 15). 1 = has moved.
bool hasMoved(const Piece _piece) {
    uint16_t movedMask = 0b1000000000000000;
    return _piece & movedMask;
}

// Sets the has-moved flag to the value passed (_has_moved).
void setMoved(Piece &_piece, bool _has_moved) {
    setSequence(_piece, 15, 1, _has_moved);
}

Piece initPiece(const uint16_t _rank, const uint16_t _file,
                PieceType _type, bool _white, bool _in_play,
                const uint16_t _id) {
    Piece p = 0b0;
    setPositionRF(_rank, _file, p);
    setType(_type, p);
    setWhite(p, _white);
    setPlay(p, _in_play);
    setID(_id, p);
    return p;
}

// Initializes all chess pieces in the piece array.
void initPieceArr(Piece (&_arr)[32]) {
    for (int i = 0; i < 2; i++) {
        bool white = (i == 1); // 2nd iteration is for white pieces.
        uint16_t backRank = white ? 0 : 7; // back rank for white and black respectively.
        uint16_t pawnRank = white ? 1 : 6;

        for (uint16_t id = 0; id < 16; id++) {
            PieceType type;
            uint16_t rank = backRank;
            uint16_t file;

            if (id <= 7) {
                type = PAWN;
                rank = pawnRank;
                file = id;
            }
            else if (id <= 9) {
                type = ROOK;
                file = (id == 8) ? A : H;
            }
            else if (id <= 11) {
                type = KNIGHT;
                file = (id == 10) ? B : G;
            }
            else if (id <= 13) {
                type = BISHOP;
                file = (id == 12) ? C : F;
            }
            else if (id == 14) {
                type = QUEEN;
                file = D;
            }
            else {
                type = KING;
                file = E;
            }

            _arr[i * 16 + id] = initPiece(rank, file, type, white, true, id);
        }
    }
}














