#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

#include <cstdint>
#include <iostream>

using std::cout; // COMMENT OUT
using std::endl; // COMMENT OUT

#include "Constants.h"

using Piece = uint16_t; // Alias for uint16_t. Makes the code
                        // nicer to read.

enum PieceType : uint16_t {
    NONE = 0, PAWN = 1, KNIGHT = 2,
    BISHOP =  3, ROOK = 4, QUEEN = 5,
    KING = 6
};

enum File : uint16_t {
    A = 0, B = 1, C = 2,
    D = 3, E = 4, F = 5,
    G = 6, H = 7
};

enum color : bool {
    BLACK = 0, WHITE = 1
};

struct positionRF {
    uint16_t rank;
    uint16_t file;
};

// in-play flag functions.
bool inPlay(const Piece _piece);
void setPlay(Piece &_piece, bool _is_playing);

// Color functions
bool isWhite(const Piece _piece);
void setWhite(Piece &_piece, bool _is_white);

// Position functions.
uint16_t retPosition(const Piece _piece);
positionRF retPositionRF(const Piece _piece);
void setPositionRF(const uint16_t _rank, const uint16_t _file, Piece& _piece);
void setPositionI(const uint16_t _index, Piece& _piece);

// RF / index conversion functions.
uint16_t RFToIndex(const positionRF _pos);
positionRF IndexToRF(const uint16_t _index);

// Type functions.
PieceType retType(const Piece _piece);
void setType(const PieceType _type, Piece& _piece);

// ID functions.
uint16_t retID(const Piece _piece);
void setID(const uint16_t _id, Piece& _piece);

// Has-moved flag functions.
bool hasMoved(const Piece _piece);
void setMoved(Piece& _piece, bool _has_moved);

// Piece initialization functions.
Piece initPiece(const uint16_t _rank, const uint16_t _file,
                PieceType _type, bool _white, bool _in_play,
                const uint16_t _id);

void initPieceArr(Piece (&_arr)[Constants::NO_PIECES]);

#endif // PIECE_H_INCLUDED
