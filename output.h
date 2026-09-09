#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include "Piece.h"
#include "Board.h"

#include<string>
using std::string;
#include <cstdlib>

string RetFEN(const Board &_board, bool _white_turn);

#endif // OUTPUT_H_INCLUDED
