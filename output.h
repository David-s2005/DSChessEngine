#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include "Piece.h"
#include "Board.h"

#include<string>
using std::string;

movement readMove(const string &_str);
string moveToStr(movement _move);

#endif // OUTPUT_H_INCLUDED
