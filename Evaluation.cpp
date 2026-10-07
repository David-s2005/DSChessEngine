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

// Determines the positioning advantage over the opposite of _color.
// Positioning value determine by piece maps in constants.h
int positioningEvaluation(const Board &_board, color _color) {
    int scoreWhite = 0;
    int scoreBlack = 0;

    for(int i = 0; i < Constants::NO_TILES; i++) {
        int index = _board.indexMap[i];
        if(index != Constants::SENTINEL) {
            Piece p = _board.pieces[index];

            if(inPlay(p) == true) {
                PieceType type = retType(p);
                color pieceColor = static_cast<color>(isWhite(p));
                // Flip index horizontally if the piece is black.
                int sq = pieceColor ? i : i ^ 56;

                if(pieceColor == WHITE) scoreWhite += pieceSquareTable[type][sq];
                else scoreBlack += pieceSquareTable[type][sq];
            }
        }
    }

    int score = scoreWhite - scoreBlack;
    return (_color == WHITE) ? score : -score;
}

// A color gets a small bonus if it has a bishop advantage over the other side.
int BishopPair(const Board &_board, color _color) {
    int noWhiteBishops = 0;
    int noBlackBishops = 0;

    for(int i = 0; i < Constants::NO_PIECES; i++) {
        Piece p = _board.pieces[i];

        if(inPlay(p) == true) {
            if(isWhite(p) == WHITE) noWhiteBishops += 1;
            else noBlackBishops += 1;
        }
    }

    int difference = noWhiteBishops - noBlackBishops;
    if(difference == 0) return 0;
    if(difference == 1) return (_color == WHITE) ? 30: -30;
    if(difference == 2) return (_color == WHITE) ? 50: -50;
}

// Evaluates the pawn positioning of _color over its opposite.
int pawnStructureEvaluation(const Board &_board, color _color) {
    int stackedWhite = pawnsStacked(_board, WHITE) - pawnsStacked(_board, BLACK);
    int isolationWhite = singlePawns(_board, WHITE) - singlePawns(_board, BLACK);
    int pawnShieldWhite = pawnShield(_board, WHITE) - pawnShield(_board, BLACK);
    int passedWhite = passedPawnScore(_board, WHITE);

    int score = stackedWhite + isolationWhite + pawnShieldWhite;
    score = (_color == WHITE) ? score : -score;

    return score + pawnsPassed(_board, _color);
}

// Not color relative. Rewards _color for keeping its pawns close to its own
// king (a closer pawn shield is a more effective shield), penalized further
// if pawns are missing entirely.
int pawnShield(const Board &_board, color _color) {
    positionRF kingPos = findKingPos(_board, _color);
    int avgPawnRankDist = 0;
    // Penalize avgPawnRankDist if some pawns are missing.
    // all 8 pawns = no penalty (0%).
    // 7->5 pawns = 25% penalty.
    // 4->0 pawns = 42% penalty.
    float pawnNoPenalty[2] = {0.75, 0.58};
    int noPawns = 0;

    for(int i = 0; i < Constants::NO_TILES; i++) {
        int pieceIndex = _board.indexMap[i];

        if(pieceIndex != Constants::SENTINEL) {
            Piece p = _board.pieces[pieceIndex];
            if(static_cast<color>(isWhite(p)) == _color && inPlay(p) &&
               retType(p) == PAWN)
            {
                noPawns ++;
                positionRF pos = retPositionRF(p);
                avgPawnRankDist += abs(pos.rank - kingPos.rank);
            }
        }
    }

    if(noPawns != 0) avgPawnRankDist = avgPawnRankDist / noPawns;
    // Multiply by 10, we are using centipawns, and i'd like the value
    // returned to be similar in scale to what the other functions return.
    avgPawnRankDist *= 10;

    if(noPawns >= 7) return -avgPawnRankDist;
    if(noPawns >= 5) return -avgPawnRankDist * pawnNoPenalty[0];
    return -avgPawnRankDist * pawnNoPenalty[1];
}

// 30% reduction in a pawns value if stacked. Not relative to the opposite color.
int pawnsStacked(const Board &_board, color _color) {
    int pawnPerFile[8] = {0};
    retPawnPerFile(_board, _color, pawnPerFile);

    int score = 0;

    for(int i = 0; i < 8; i++) {
        if(pawnPerFile[i] > 1) {
            score -= 30 * pawnPerFile[i];
        }
    }

    return score;
}

// Bonus for each pawn that no enemy pawn can ever block or capture on its
// way to promotion. Scaled by how far advanced it already is.
int passedPawnScore(const Board &_board, color _color) {
    int score = 0;

    for(int i = 0; i < Constants::NO_PIECES; i++) {
        Piece p = _board.pieces[i];
        if(!inPlay(p) || retType(p) != PAWN || isWhite(p) != _color) continue;

        positionRF pos = retPositionRF(p);
        bool passed = true;

        for(int j = 0; j < Constants::NO_PIECES; j++) {
            Piece enemy = _board.pieces[j];
            if(!inPlay(enemy) || retType(enemy) != PAWN || isWhite(enemy) == _color) continue;

            positionRF enemyPos = retPositionRF(enemy);
            int fileDist = abs(static_cast<int>(enemyPos.file) - static_cast<int>(pos.file));
            if(fileDist > 1) continue;

            // Same file blocks advancing pawn directly, the adjacent files of the blocking
            // pawn also prevent the pawn from advancing safely.
            bool enemyAhead = (_color == WHITE) ? (enemyPos.rank > pos.rank)
                                                  : (enemyPos.rank < pos.rank);
            if(enemyAhead) {
                passed = false;
                break;
            }
        }

        if(passed) {
            int rankIndex = (_color == WHITE) ? pos.rank : (7 - pos.rank);
            score += passedPawnBonus[rankIndex];
        }
    }

    return score;
}

int pawnsPassed(const Board &_board, color _color) {
    int scoreWhite = passedPawnScore(_board, WHITE);
    int scoreBlack = passedPawnScore(_board, BLACK);

    int score = scoreWhite - scoreBlack;
    return (_color == WHITE) ? score : -score;
}

void retPawnPerFile(const Board &_board, color _color, int pawnPerFile[8]) {
    for(int i = 0; i < Constants::NO_PIECES; i++) {
        Piece p = _board.pieces[i];

        if(inPlay(p) && retType(p) == PAWN && isWhite(p) == _color) {
            positionRF pos = retPositionRF(p);
            pawnPerFile[pos.file] += 1;
        }
    }
}

// Isolated on one side = 13% penalty
// Isolated on both sides = 26% penalty
// Not scaled relative to the opposite color.
int singlePawns(const Board &_board, color _color) {
    int pawnPerFile[8] = {0};
    retPawnPerFile(_board, _color, pawnPerFile);

    int score = 0;

    // Special cases A & H.
    if(pawnPerFile[0] != 0 && pawnPerFile[1] == 0) score -= 13;
    if(pawnPerFile[0] != 0 && pawnPerFile[7] == 0) score -= 13;

    // Visit files B -> G.
    for(int i = 1; i < 7; i++) {
        int sidesIsolated = 0;
        if(pawnPerFile[i] > 0) {
            if(pawnPerFile[i - 1] == 0) sidesIsolated += 1;
            if(pawnPerFile[i + 1] == 0) sidesIsolated += 1;
        }

        if(sidesIsolated == 1) score -= 13;
        else if(sidesIsolated == 2) score -= 26;
    }

    return score;
}

// returns a weighted score of all evaluating functions for a board.
float heuristic(const Board &_board, color _color) {
    int mat = materialEvaluation(_board, _color);
    int mob = mobilityEvaluation(_board, _color);
    int pos = positioningEvaluation(_board, _color);
    int pawnPos = pawnStructureEvaluation(_board, _color);

    return mat + (mob * 0.15) + (pos * 0.35) + (pawnPos * 0.25);
}
