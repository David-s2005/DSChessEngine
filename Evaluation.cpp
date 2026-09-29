#include "Evaluation.h"
#include "Move_Generator.h"

// Returns a int representing the advantage _color has over the other side.
// < 0 _color is at a disadvantage
// > 0 _color is at an advantage
// The further away from the threshold, the greater the advantage.
int materialEvaluation(const Board &_board, color _color) {
    int scoreWhite = 0;
    int scoreBlack = 0;

    for(int i = 0; i < Constants::NO_PIECES; i++) {
        if(inPlay(_board.pieces[i]) == true) {
            if(static_cast<color>(isWhite(_board.pieces[i])) == WHITE) {
                scoreWhite += materialValues[retType(_board.pieces[i])];
            }
            else {
                scoreBlack += materialValues[retType(_board.pieces[i])];
            }
        }
    }

    int score = scoreWhite - scoreBlack;
    return (_color == WHITE) ? score : -score;
}

// returns the total number number of moves _color has over its enemy.
// greater is better.
int mobilityEvaluation(const Board &_board, color _color) {
    movement movesWhite[218];
    int totalMovesWhite = 0;
    movement movesBlack[218];
    int totalMovesBlack = 0;

    generateLegalMoves(_board, WHITE, movesWhite, totalMovesWhite);
    generateLegalMoves(_board, BLACK, movesBlack, totalMovesBlack);

    int score = totalMovesWhite - totalMovesBlack;

    return (_color == WHITE) ? score : -score;
}

// returns a weighted score of all evaluating functions for a board.
float heuristic(const Board &_board, color _color) {
    int mat = materialEvaluation(_board, _color);
    int mob = mobilityEvaluation(_board, _color);

    return (mat * 0.75) + (mob * 0.25);
}
