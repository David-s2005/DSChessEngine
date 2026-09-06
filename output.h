#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include<string>
using std::string;

#include "Piece.h"

string RetFEN(const Piece (&_arr)[32], bool _white_turn);

#endif // OUTPUT_H_INCLUDED
