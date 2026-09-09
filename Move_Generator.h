#ifndef MOVE_GENERATOR_H_INCLUDED
#define MOVE_GENERATOR_H_INCLUDED

#include "Board.h"

bool canMoveHere(const Board &_board, positionRF _position);

// Moves knight. _piece_index is the index of the thing we're trying to move.
// _board is the board state. _moves is a array of what actions we can make here.
// & total moves is the length of _moves.
void generateMovesKnight(uint16_t _piece_index, Board &_board,
                positionRF (&_moves)[], uint16_t &_total_moves);

#endif // MOVE_GENERATOR_H_INCLUDED
