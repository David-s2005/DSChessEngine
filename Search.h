#ifndef SEARCH_H_INCLUDED
#define SEARCH_H_INCLUDED

#include "Board.h"

int negamax(const Board &_board, color _sideToMove, int _depth,
            int _alpha = Constants::CHECKMATE_SCORE,
            int _beta = -Constants::CHECKMATE_SCORE);

movement findBestMove(const Board &_board, color _sideToMove, int _depth);

void sortByMMVLVA(const Board &_board, movement (&_moves)[], int _total);
int MMVLVA(movement move, const Board &_board);
int appendKillerMoves(const Board &_board, movement (&_moves)[], int _total, int _depth);
void sortByHistory(movement (&_moves)[], int _start, int _total);
void sort(const Board &_board, movement (&_moves)[], int _total, int _depth);
void resetKillerMoves();
void initKillerMoves();
void updateHistory(movement _move, int _depth);
int retHistoryScore(movement _move);
void initHistory();
int timeBudget(int noMovesLeft, int timeLeft);
movement iterativeDeepening(const Board &_board, int _timeBudget, color _sideToMove);


#endif // SEARCH_H_INCLUDED
