#ifndef MOVE_GENERATOR_H_INCLUDED
#define MOVE_GENERATOR_H_INCLUDED

#include "Board.h"

#include <cstdlib>



// What's on a given square, relative to the color trying to move there.
// Plain enum (not enum class) so it stays comparable to the raw codes
// Test.cpp already checks canMoveHere's result against.
enum MoveStatus : uint16_t {
    OffBoard = 0,
    OwnPiece = 1,
    Empty = 2,
    Capture = 3
};

// enum over the castling rights boolean array in each board instance.
enum CASTLINGRIGHTS : uint16_t {
    BLACKQUEENSIDE = 0,
    BLACKKINGSIDE = 1,
    WHITEQUEENSIDE = 2,
    WHITEKINGSIDE = 3
};

MoveStatus canMoveHere(const Board &_board, const positionRF _position, bool _isWhite);

void generateMovesKnight(uint16_t _piece_index, const Board &_board,
                         movement (&_moves)[], uint16_t &_total_moves);

void generateMovesKing(uint16_t _piece_index, const Board &_board,
                       movement (&_moves)[], uint16_t &_total_moves);

void generateMovesBishop(uint16_t _piece_index, const Board &_board,
                          movement (&_moves)[], uint16_t &_total_moves);

void generateMovesPawn(uint16_t _piece_index, const Board &_board,
                          movement (&_moves)[], uint16_t &_total_moves);

void generateMovesRook(uint16_t _piece_index, const Board &_board,
                       movement (&_moves)[], uint16_t &_total_moves);

void generateMovesQueen(uint16_t _piece_index, const Board &_board,
                       movement (&_moves)[], uint16_t &_total_moves);

void generateMovesForSide(const Board &_board, color _isWhite,
                          movement (&_moves)[], uint16_t _total_moves,
                          movement (&_legal_moves)[], uint16_t &_total_legal_moves);

void generateLegalMoves(const Board &_board, color _isWhite,
                        movement (&_moves)[], int &_total_moves);

void movePiece(Board &_board, movement _move);

void retAttackPawn(const Board &_board, positionRF _pawnPos, movement (&_attacks)[2]);

void retAttackKing(positionRF _kingPos, movement (&_attacks)[8]);

bool isSquareAttacked(const Board &_board, positionRF _position, color _attacking_color);

bool containsPosition(positionRF _pos, const movement *_moves, uint16_t _noMoves);

void updateCastlingRights(Board &_board);

positionRF findKingPos(const Board &_board, color _isWhite);

bool canCastle(const Board &_board,
               positionRF _kingPos, positionRF _rookPos,
               const positionRF *_emptySquares, int _noEmptySquares,
               const positionRF *_safeSquares, int _noSafeSquares,
               color _attackingColor);

bool isLegalMove(movement _move, const movement (&_moves)[], int _noMoves);

#endif // MOVE_GENERATOR_H_INCLUDED
