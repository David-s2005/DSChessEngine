#ifndef EVALUATION_H_INCLUDED
#define EVALUATION_H_INCLUDED

#include "Piece.h"
#include "Board.h"
#include "Constants.h"

int materialEvaluation(const Board &_board, color _color);
int mobilityEvaluation(const Board &_board, color _color);
float heuristic(const Board &_board, color _color);
int positioningEvaluation(const Board &_board, color _color);
int BishopPair(const Board &_board, color _color);
int pawnsStacked(const Board &_board, color _color);
int pawnStructureEvaluation(const Board &_board, color _color);
int singlePawns(const Board &_board, color _color);
void retPawnPerFile(const Board &_board, color _color, int pawnPerFile[8]);
int passedPawnScore(const Board &_board, color _color);
int pawnsPassed(const Board &_board, color _color);
int pawnShield(const Board &_board, color _color);

#endif // EVALUATION_H_INCLUDED
