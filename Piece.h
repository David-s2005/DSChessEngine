#ifndef PIECE_H_INCLUDED
#define PIECE_H_INCLUDED

#include <cstdint>

using Piece = uint16_t; // Alias for uint16_t. Makes the code
                        // nicer to read.

enum PieceType : uint16_t {
    NONE = 0, PAWN = 1, KNIGHT = 2,
    BISHOP =  3, ROOK = 4, QUEEN = 5,
    KING = 6
};

bool inPlay(const Piece _piece);

bool isWhite(Piece _piece);

uint16_t retPosition(PieceType _piece);
void setPositionRF(const uint16_t _rank, const uint16_t _file);
void setPositionI(const uint16_t _index);

uint16_t retType(PieceType _piece);
void setType(const PieceType _type);

Piece initPiece(PieceType _piece);

#endif // PIECE_H_INCLUDED
