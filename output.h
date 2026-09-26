#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include "Piece.h"
#include "Board.h"

#include<string>
using std::string;

string RetFEN(const Board &_board, bool _white_turn);
movement readMove(const string &_str);

#endif // OUTPUT_H_INCLUDED
