#include "Piece.h"
#include "Bitwise_Logic.h"

// Returns a boolean based on where the 1st bit is 1 or 0.
bool inPlay(const Piece _piece) {
    return retSequence(_piece, Constants::INPLAY_BIT, 1);
}

// Sets the is playing bit to the value passed (_is_playing).
void setPlay(Piece &_piece, bool _is_playing) {
    setSequence(_piece, Constants::INPLAY_BIT, 1, _is_playing);
}

// Checks if the color bit (bit 1) is white or not. 1 = white.
bool isWhite(const Piece _piece) {
    return retSequence(_piece, Constants::COLOR_BIT, 1);
}

// Sets the white flag to the value passed (_is_white).
void setWhite(Piece &_piece, bool _is_white) {
    setSequence(_piece, Constants::COLOR_BIT, 1, _is_white);
}

// Returns the 6 bits (5-10) of the pieces 16 bit sequence that represent
// the pieces position (0-63, covering all squares of an 8x8 board).
uint16_t retPosition(const Piece _piece) {
    return retSequence(_piece, Constants::POS_START, Constants::POS_LEN);
}

// Returns the position of a given piece in rank-file form.
positionRF retPositionRF(const Piece _piece) {
    return IndexToRF(retPosition(_piece));
}

// Sets the position of the provided piece by rank and file. Decodes RF
// into an index the 16 bit piece number can accept.
void setPositionRF(const uint16_t _rank, const uint16_t _file, Piece &_piece) {
    uint16_t index = RFToIndex({_rank, _file});
    setSequence(_piece, Constants::POS_START, Constants::POS_LEN, index);
}

// Sets position by index, remember that position is a 0-63 value.
void setPositionI(const uint16_t _index, Piece &_piece) {
    setSequence(_piece, Constants::POS_START, Constants::POS_LEN, _index);
}

uint16_t RFToIndex(const positionRF _pos) {
    return static_cast<uint16_t>((_pos.rank * Constants::BOARD_SIDE_LEN) + _pos.file);
}

positionRF IndexToRF(const uint16_t _index) {
    positionRF retPos;
    retPos.file = static_cast<uint16_t>(_index % Constants::BOARD_SIDE_LEN);
    retPos.rank = static_cast<uint16_t>(_index / Constants::BOARD_SIDE_LEN);
    return retPos;
}

// Returns the 3 bits (2-4) that represent the piece's type.
PieceType retType(const Piece _piece) {
    return static_cast<PieceType>(retSequence(_piece, Constants::TYPE_START, Constants::TYPE_LEN));
}

// Sets the 3 bits (2-4) that represent the piece's type.
void setType(const PieceType _type, Piece &_piece) {
    setSequence(_piece, Constants::TYPE_START, Constants::TYPE_LEN, static_cast<uint16_t>(_type));
}

// Returns the 4 bits (11-14) used alongside the color flag to
// uniquely identify a piece.
uint16_t retID(const Piece _piece) {
    return retSequence(_piece, Constants::ID_START, Constants::ID_LEN);
}

// Sets the 4 bits (11-14) used alongside the color flag to
// uniquely identify a piece.
void setID(const uint16_t _id, Piece &_piece) {
    setSequence(_piece, Constants::ID_START, Constants::ID_LEN, _id);
}

// Checks the has-moved flag (bit 15). 1 = has moved.
bool hasMoved(const Piece _piece) {
    return retSequence(_piece, Constants::MOVED_BIT, 1);
}

// Sets the has-moved flag to the value passed (_has_moved).
void setMoved(Piece &_piece, bool _has_moved) {
    setSequence(_piece, Constants::MOVED_BIT, 1, _has_moved);
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
void initPieceArr(Piece (&_arr)[Constants::NO_PIECES]) {
    for (int i = 0; i < 2; i++) {
        bool white = (i == 1); // 2nd iteration is for white pieces.
        // back rank for white and black respectively.
        uint16_t backRank = white ? 0 : Constants::BOARD_SIDE_LEN - 1;
        uint16_t pawnRank = white ? Constants::WHITE_PAWN_START_RANK
                                  : Constants::BLACK_PAWN_START_RANK;

        for (uint16_t id = 0; id < Constants::NO_PIECES / 2; id++) {
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

            _arr[i * (Constants::NO_PIECES / 2) + id] = initPiece(rank, file, type, white, true, id);
        }
    }
}














