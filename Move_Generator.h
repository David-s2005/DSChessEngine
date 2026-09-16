#ifndef MOVE_GENERATOR_H_INCLUDED
#define MOVE_GENERATOR_H_INCLUDED

#include "Board.h"

struct movement {
    positionRF start;
    positionRF end;
};

uint16_t canMoveHere(const Board &_board, const positionRF _position, bool _isWhite);

void generateMovesKnight(uint16_t _piece_index, Board &_board,
                         movement (&_moves)[], uint16_t &_total_moves);

void generateMovesKing(uint16_t _piece_index, Board &_board,
                       movement (&_moves)[], uint16_t &_total_moves);

void generateMovesBishop(uint16_t _piece_index, Board &_board,
                          movement (&_moves)[], uint16_t &_total_moves);

#endif // MOVE_GENERATOR_H_INCLUDED
