#include <limits>
#include <vector>
#include <array>
#include <chrono>

#include "Search.h"
#include "Evaluation.h"
#include "Move_Generator.h"

// We are using a vector because the depth is determined
// at runtime.
static std::vector<std::array<movement, 2>> killerMoves;

// Stores a score for a movement. 4096 elements. Think of this
// as a dictionary [startpos][endpos] is the key, score is the value.
static int historyTable[Constants::NO_TILES][Constants::NO_TILES];

// A bit of a gross global variable, but preferable over passing down
// the deadline multiple functions. We need to see the remaining time
// whilst searching, not before or after.
static std::chrono::steady_clock::time_point searchDeadline;
static bool searchHasDeadline = false;
static bool searchAborted = false;

static bool deadlinePassed() {
    if(!searchHasDeadline) return false;
    if(std::chrono::steady_clock::now() >= searchDeadline) {
        searchAborted = true;
        return true;
    }
    return false;
}

// Start search with a depth of 1, and get deeper and deeper until we've ran out of time.
movement iterativeDeepening(const Board &_board, int _timeBudget, color _sideToMove) {
    searchHasDeadline = true;
    // Deadline = now + time budget.
    searchDeadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(_timeBudget);

    int depth = 1;
    movement bestMove{};
    bool haveCompletedPass = false;

    while(true) {
        // KillerMoves uses the default depth of 4. Iterative deepening
        // can reach depths greater than 4, so it needs to be resized
        // to our current depth.
        if(static_cast<int>(killerMoves.size()) <= depth) {
            killerMoves.resize(depth + 1);
        }
        Constants::MAX_DEPTH = depth;

        searchAborted = false;
        movement candidate = findBestMove(_board, _sideToMove, depth);

        // Search at this depth complete, save this depths best move.
        if(!searchAborted) {
            bestMove = candidate;
            haveCompletedPass = true;
        }
        // We've ran out of time during the search for this depth.
        else if(!haveCompletedPass) {
            bestMove = candidate;
        }

        if(searchAborted || deadlinePassed()) {
            break;
        }
        depth++;
    }

    searchHasDeadline = false;
    return bestMove;
}

// Bottom up search of the game tree from _depth. Negamax algorithm with alpha-beta pruning.
int negamax(const Board &_board, color _sideToMove, int _depth, int _alpha, int _beta) {
    // We've ran out of time, return nothing as we haven't reached the bottom.
    if(deadlinePassed()) {
        return 0;
    }

    // We've reached the bottom. Start propagating up.
    if(_depth == 0) {
        return heuristic(_board, _sideToMove);
    }

    // Moves are the branches of this node.
    movement moves[218];
    int total = 0;
    generateLegalMoves(_board, _sideToMove, moves, total);
    sort(_board, moves, total, _depth);

    // No moves were generated, meaning we've reached a stalemate or checkmate.
    if(total == 0) {
        positionRF kingPos = findKingPos(_board, _sideToMove);
        if(isSquareAttacked(_board, kingPos, static_cast<color>(!_sideToMove)) == true) {
            return Constants::CHECKMATE_SCORE;
        }
        else return 0;
    }

    int best = Constants::CHECKMATE_SCORE;

    for(int i = 0; i < total; i++) {
        Board copy = _board;
        movePiece(copy, moves[i]);
        // Chess is zero-sum (Whats good for us is equally bad for them). So negate the score.
        // Pass the parents alpha as the child's beta and vice versa for the child's alpha.
        int score = -negamax(copy, static_cast<color>(!_sideToMove), _depth - 1, -_beta, -_alpha);
        if(score > best) best = score;
        if(best > _alpha) _alpha = best;

        // Beta cutoff. The opponent (parent node) has a superior value so don't evaluate this path
        // further. A optimal opponent will never choose this path as it has already found a route
        // with a superior value for it.
        if(_alpha >= _beta) {
            // Record our most recent killer move & bump this move's history score.
            if(canMoveHere(_board, moves[i].end, _sideToMove) != Capture) {
                killerMoves[_depth][1] = killerMoves[_depth][0];
                killerMoves[_depth][0] = moves[i];
                updateHistory(moves[i], _depth);
            }
            break;
        }
    }
    return best;
}

movement findBestMove(const Board &_board, color _sideToMove, int _depth) {
    movement moves[218];
    int total = 0;
    generateLegalMoves(_board, _sideToMove, moves, total);

    movement bestMove = moves[0];
    int bestScore = Constants::CHECKMATE_SCORE;

    for(int i = 0; i < total; i++) {
        if(searchAborted) break;

        Board copy = _board;
        movePiece(copy, moves[i]);
        int score = -negamax(copy, static_cast<color>(!_sideToMove), _depth - 1);

        // We've hit the deadline, don't record the current score as it is useless.
        if(searchAborted) break;

        if(score > bestScore) {
            bestScore = score;
            bestMove = moves[i];
        }
    }

    return bestMove;
}

void initKillerMoves() {
    killerMoves.assign(Constants::MAX_DEPTH, std::array<movement, 2>{});
}

// Sorts by MMV/LVA, then moves killer moves just beneath the MMV/LVA moves,
// then ranks everything left (the remaining quiet moves) by history score.
void sort(const Board &_board, movement (&_moves)[], int _total, int _depth) {
    sortByMMVLVA(_board, _moves, _total);
    int quietStart = appendKillerMoves(_board, _moves, _total, _depth);
    sortByHistory(_moves, quietStart, _total);
}

// Adds the killer moves immediately after moves sorted by MMV/LVA. Returns
// the index where the remaining (non-capture, non-killer) quiet moves start.
int appendKillerMoves(const Board &_board, movement (&_moves)[], int _total, int _depth) {
    // Find where the capture block ends.
    int captureEnd = 0;
    while(captureEnd < _total && MMVLVA(_moves[captureEnd], _board) > 0) {
        captureEnd++;
    }

    // Try both killer slots at this depth, swap if present.
    for(int k = 0; k < 2; k++) {
        movement killer = killerMoves[_depth][k];

        for(int i = captureEnd; i < _total; i++) {
            if(_moves[i].start.rank == killer.start.rank &&
               _moves[i].start.file == killer.start.file &&
               _moves[i].end.rank == killer.end.rank &&
               _moves[i].end.file == killer.end.file)
            {
                movement temp = _moves[captureEnd];
                _moves[captureEnd] = _moves[i];
                _moves[i] = temp;
                captureEnd++;
                break;
            }
        }
    }

    return captureEnd;
}

// Sorts _moves[_start.._total) in descending order by history score. Placed
// after killer scores.
void sortByHistory(movement (&_moves)[], int _start, int _total) {
    for(int i = _start + 1; i < _total; i++) {
        movement key = _moves[i];
        int keyScore = retHistoryScore(key);

        int j = i - 1;
        while(j >= _start && retHistoryScore(_moves[j]) < keyScore) {
            _moves[j + 1] = _moves[j];
            j--;
        }
        _moves[j + 1] = key;
    }
}

void resetKillerMoves() {
    for(int d = 0; d < Constants::MAX_DEPTH; d++) {
        killerMoves[d][0] = {};
        killerMoves[d][1] = {};
    }
}

// Sorts the array in descending order by MMVLVA. Uses a simple insertion
// sort to do so.
void sortByMMVLVA(const Board &_board, movement (&_moves)[], int _total) {
    for(int i = 1; i < _total; i++) {
        movement key = _moves[i];
        int pair = MMVLVA(_moves[i], _board);

        int j = i - 1;
        // Work backwards from i until we find a element that has a greater
        // value than our current move (key).
        while(j >= 0 && MMVLVA(_moves[j], _board) < pair) {
            _moves[j + 1] = _moves[j];
            j--;
        }
        _moves[j + 1] = key;
    }
}

// Returns 0 if we are attacking into nothing, return the material score of the
// victim minus the aggressors material value.
int MMVLVA(movement move, const Board &_board) {
    Piece aggressor = _board.pieces[_board.indexMap[RFToIndex(move.start)]];
    uint16_t end_index = _board.indexMap[RFToIndex(move.end)];

    if(end_index == Constants::SENTINEL) {
        return 0;
    }

    Piece victim = _board.pieces[end_index];

    // + 1 to ensure that captures appear above killer moves (queen taking a queen = 0.)
    return materialValues[retType(victim)] - materialValues[retType(aggressor)] + 1;
}

void initHistory() {
    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 64; j++) {
            // -1 = unseen.
            historyTable[i][j] = -1;
        }
    }
}

int retHistoryScore(movement _move) {
    return historyTable[RFToIndex(_move.start)][RFToIndex(_move.end)];
}

// recurring moves closer to the bottom of the tree are scored higher. score = _depth^2.
void updateHistory(movement _move, int _depth) {
    historyTable[RFToIndex(_move.start)][RFToIndex(_move.end)] += _depth * _depth;
}
