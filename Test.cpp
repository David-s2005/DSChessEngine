#include <iostream>

#include "Piece.h"
#include "Bitwise_Logic.h"
#include "Board.h"
#include "output.h"
#include "Constants.h"
#include "Move_Generator.h"
#include "Evaluation.h"

using std::cout;
using std::endl;

// --- BITWISE LOGIC TESTS ---

// Test if retSequence returns a valid sequence.
bool test_1() {
    uint16_t test = 0b0001010110000000;
    uint16_t ret_val = retSequence(test, 3, 6);
    return ret_val == 48;
}

// Check is setSequence places a binary sequence into the correct position.
bool test_2() {
    uint16_t seq = 0b0001010110000000;
    uint16_t val = retSequence(seq, 3, 6) / 2;
    setSequence(seq, 3, 6, val); // insert above result back og spot.
    return retSequence(seq, 3, 6) == 24;
}

// --- PIECE TESTS ---

// Simple piece init test.
bool test_3() {
    // White rook at C3 that is in play.
    Piece p = initPiece(C, 3, ROOK, true, true, 1);

    // Just checking sanity. Make sure we don't crash.
    return true;
}

// Check if isWhite works.
bool test_4() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1); // white rook.
    Piece p2 = initPiece(C, 4, ROOK, false, true, 2);// black rook.

    return (isWhite(p1) == true &&
            isWhite(p2) == false);
}

// i know its not sequential, but whatever lol.

// Check if setWhite is working.
bool test_7() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1); // white rook.
    setWhite(p1, false); // Make rook black.

    return isWhite(p1) == false;
}

// Check if inPlay works.
bool test_5() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1); // white rook.
    Piece p2 = initPiece(C, 4, ROOK, false, false, 2);// non playing.

    return inPlay(p2) == false && inPlay(p1) == true;
}

// Check if setPlay works.
bool test_6() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1);
    setPlay(p1, false); // set to not playing.

    return inPlay(p1) == false;
}
// --- POSITION FUNCTION TESTS ---

// Check if retPosition returns a position index.
bool test_8() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1);
    uint16_t pos = retPosition(p1);

    return pos == 19;
}

// Check if retPositionRF returns C3.
bool test_9() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1);
    positionRF RF = retPositionRF(p1);

    return RF.rank == C && RF.file == 3;
}

// Check if setPositionRF sets the pieces position to the passed value.
bool test_10() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1);

    setPositionRF(E, 5, p1);

    positionRF RF = retPositionRF(p1);

    return RF.rank == E && RF.file == 5;
}

// Check that setPositionI sets the piece to A1.
bool test_11() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1);

    setPositionI(0, p1); // goto A1.

    positionRF RF = retPositionRF(p1);

    return RF.rank == A && RF.file == 0;
}

// Check if RFToIndex returns a valid index from RF.
bool test_12() {
    positionRF RF {2, C};
    uint16_t i = RFToIndex(RF);

    return i == 18;
}

// IndexToRF test. Validate it returns a correct index.
bool test_13() {
    uint16_t index = 9;
    positionRF pos = IndexToRF(index);

    return pos.file == B &&
           pos.rank == 1;
}

// ---PIECE SETTERS & GETTER TESTS---

// Validate retType returns valid data.
bool test_14() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1);

    uint16_t type = retType(p1);

    return type == ROOK;
}

// setType test.
bool test_15() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1);

    setType(QUEEN, p1);
    uint16_t type = retType(p1);

    return type == QUEEN;
}

// retID test.
bool test_16() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1);

    uint16_t ID = retID(p1);

    return ID == 1;
}

// setID test.
bool test_17() {
    Piece p1 = initPiece(C, 3, ROOK, true, true, 1);

    setID(999, p1);

    return retID(p1) == 999;
}

// setMoved test.
bool test_18() {
    Piece p1 = initPiece(3, C, ROOK, true, true, 1); // hasn't moved by default
    setMoved(p1, true);
    uint16_t bitseq = retSequence(p1, 15, 1); // get has moved get. (15th bit)

    return bitseq == 1;
}

// hasMoved test.
bool test_19() {
    Piece p1 = initPiece(3, C, ROOK, true, true, 1); // hasn't moved.
    bool moved_1 = hasMoved(p1);
    setMoved(p1, true);
    bool moved_2 = hasMoved(p1);

    return !(moved_1) && moved_2;
}

// initPieceArr skipped because it has been used multiple times at
// this point.

// updateLookupMap test. Occasionally fails because we dont initialize
// all pieces.
bool test_20() {
    Board b;
    positionRF pos = {A, 5};

    b.pieces[0] = initPiece(A, 5, ROOK, true, true, 1);
    updateLookupMap(b);

    uint16_t index = RFToIndex(pos); // should return 0.
    return b.indexMap[index] == 0;
}

// initPieceArr test. Verify visually. Note that king and queen
// have swapped places. This is fine as showBoard works top-down
// from H8.
bool test_21() {
    Board b;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    cout << "Test 21 Board: " << endl;
    showBoard(b);

    return true;
}

// updateLookupMap test. All rooks should be removed.
bool test_22() {
    Board b;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF rook1 = {7, A};
    positionRF rook2 = {7, H};
    positionRF rook3 = {0, A};
    positionRF rook4 = {0, H};


    uint16_t rook_1_index = b.indexMap[RFToIndex(rook1)];
    uint16_t rook_2_index = b.indexMap[RFToIndex(rook2)];
    uint16_t rook_3_index = b.indexMap[RFToIndex(rook3)];
    uint16_t rook_4_index = b.indexMap[RFToIndex(rook4)];

    setPlay(b.pieces[rook_1_index], false);
    setPlay(b.pieces[rook_2_index], false);
    setPlay(b.pieces[rook_3_index], false);
    setPlay(b.pieces[rook_4_index], false);

    updateLookupMap(b);

    cout << "Test 22 Board: " << endl;
    showBoard(b);

    return true;
}

// King movement test
bool test_23() {
    movement movements[218]; // 218 moves possible in a single turn.
    uint16_t NoMovements = 0;


    Board b;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF BlackKingPos = {7, E};
    uint16_t index = b.indexMap[RFToIndex(BlackKingPos)];

    generateMovesKing(index, b, movements, NoMovements);

    return NoMovements == 0; // King cant move at the start
}

// Simple bishop move test.
bool test_24() {
    movement movements[218];
    uint16_t NoMovements = 0;

    Board b;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF BlackBishopPos = {7, G}; // Black bishop.
    uint16_t index = b.indexMap[RFToIndex(BlackBishopPos)];

    generateMovesBishop(index, b, movements, NoMovements);

    return NoMovements == 0; // Bishops can move at the start.
}

// Simple knight movement test.
bool test_25() {
    movement movements[218];
    uint16_t NoMovements = 0;

    Board b;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF BlackBishopPos = {7, F}; // Black bishop.
    uint16_t index = b.indexMap[RFToIndex(BlackBishopPos)];

    generateMovesKnight(index, b, movements, NoMovements);

    return NoMovements == 2; // All knights can have only 2 start moves.
}

// Simple pawn movement test.
bool test_26() {
    movement movements[218];
    uint16_t NoMovements = 0;

    Board b;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF BlackPawnPos = {6, A}; // Black pawn.
    uint16_t index = b.indexMap[RFToIndex(BlackPawnPos)];

    generateMovesPawn(index, b, movements, NoMovements);

    return NoMovements == 2; // All pawns can make 2 moves at the start.
}

// Rook movement test.
bool test_27() {
    movement movements[218];
    uint16_t NoMovements = 0;

    Board b;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF BlackRookPos = {7, A}; // Queenside black rook.
    uint16_t index = b.indexMap[RFToIndex(BlackRookPos)];

    generateMovesRook(index, b, movements, NoMovements);

    return NoMovements == 0;
}

// Queen movement test.
bool test_28() {
    movement movements[218];
    uint16_t NoMovements = 0;

    Board b;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF BlackQueenPos = {7, D}; // Black queen.
    uint16_t index = b.indexMap[RFToIndex(BlackQueenPos)];

    generateMovesQueen(index, b, movements, NoMovements);

    return NoMovements == 0;
}

bool test_29() {
    movement movements[218];
    uint16_t NoMovements = 0;

    Board b;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF blackPawnPos = {6, A};
    uint16_t index = b.indexMap[RFToIndex(blackPawnPos)];

    generateMovesPawn(index, b, movements, NoMovements);

    movePiece(b, movements[0]);

    cout << "Test 29 Board: " << endl;
    showBoard(b);

    return true; // visual validation.
}

// Pawn movement promotion test.
bool test_30() {
    Board b;
    movement movements[218];
    uint16_t NoMovements = 0;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    movement move = {{6, A}, {1, A}};
    movePiece(b, move);
    uint16_t pieceIndex = b.indexMap[RFToIndex({1, A})];
    Piece &queen = b.pieces[pieceIndex];

    cout << "INDEX: " << pieceIndex << endl;

    generateMovesPawn(pieceIndex, b, movements, NoMovements);

    movePiece(b, movements[0]); // promote to queen.

    return retType(queen) == QUEEN;
}

// Simple en passant test.
bool test_32() {
    Board b;
    movement movements[218];
    uint16_t NoMovements = 0;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF blackPawnPos = {6, A};

    generateMovesPawn(b.indexMap[RFToIndex(blackPawnPos)], b, movements, NoMovements);

    movePiece(b, movements[1]); // Double jump advance.

    cout << "Test 32 board: " << endl;
    //showBoard(b);

    return (b.EnPassantTarget.rank == 5 &&
            b.EnPassantTarget.file == A);
}

// retPawnAttack test.
bool test_33() {
    Board b;
    movement movements[2];
    uint16_t NoMovements = 0;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF whitePawnPos = {1, A};

    retAttackPawn(b, whitePawnPos, movements);

    //cout << "Movement 1 rank: " << movements[0].end.rank << endl;
    //cout << "Movement 1 file: " << movements[0].end.file << endl;
    //cout << "Movement 2 rank: " << movements[1].end.rank << endl;
    //cout << "Movement 2 file: " << movements[1].end.file << endl;

    return movements[1].end.rank == Constants::SENTINEL &&
           movements[1].end.file == Constants::SENTINEL &&
           movements[0].end.rank == 2 &&
           movements[0].end.file == B;
}

// Simple retKingAttack test.
// Visually validate by ensuring 3 sentinel tiles are detected.
bool test_34() {
    Board b;
    movement movements[8];
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF whiteKingPos = {0, E};

    retAttackKing(whiteKingPos, movements);

    for(int i = 0; i < 8; i++) {
        cout << "Rank: " << movements[i].end.rank << endl;
        cout << "File: " << movements[i].end.file << endl;
        cout << endl;
    }

    return true;
}

bool test_35() {
    Board b;
    movement movements[8];
    initPieceArr(b.pieces);
    updateLookupMap(b);

    positionRF whiteKingPos = {0, E};

    bool attacked1 = isSquareAttacked(b, whiteKingPos, BLACK);

    return attacked1 == false;
}

// Board setup test: white king and both white rooks on their starting
// squares, nothing else on the board.
bool test_36() {
    Board b;
    for(uint16_t i = 0; i < Constants::NO_PIECES; i++) {
        b.pieces[i] = initPiece(0, 0, NONE, true, false, i);
    }

    positionRF whiteKingPos = {0, E};
    positionRF whiteQueensideRookPos = {0, A};
    positionRF whiteKingsideRookPos = {0, H};

    b.pieces[0] = initPiece(whiteKingPos.rank, whiteKingPos.file, KING, true, true, 0);
    b.pieces[1] = initPiece(whiteQueensideRookPos.rank, whiteQueensideRookPos.file, ROOK, true, true, 1);
    b.pieces[2] = initPiece(whiteKingsideRookPos.rank, whiteKingsideRookPos.file, ROOK, true, true, 2);

    updateLookupMap(b);

    return b.indexMap[RFToIndex(whiteKingPos)] == 0 &&
           b.indexMap[RFToIndex(whiteQueensideRookPos)] == 1 &&
           b.indexMap[RFToIndex(whiteKingsideRookPos)] == 2;
}

// Material evaluation test. Should return 0, no advantage on either side.
bool test_37() {
    Board b;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    return materialEvaluation(b, WHITE) == 0;
}

// Mobility evaluation test. Should return 0.
bool test_38() {
    Board b;
    initPieceArr(b.pieces);
    updateLookupMap(b);

    return mobilityEvaluation(b, WHITE) == 0;
}

void runTest(const char *_name, bool _result) {
    if (_result) {
        cout << _name << " Passed!" << endl;
    }
    else {
        cout << "!!! " << _name << " Failed !!!" << endl;
    }
}

int main() {
    runTest("Test 1", test_1());
    runTest("Test 2", test_2());
    runTest("Test 3", test_3());
    runTest("Test 4", test_4());
    runTest("Test 5", test_5());
    runTest("Test 6", test_6());
    runTest("Test 7", test_7());
    runTest("Test 8", test_8());
    runTest("Test 9", test_9());
    runTest("Test 10", test_10());
    runTest("Test 11", test_11());
    runTest("Test 12", test_12());
    runTest("Test 13", test_13());
    runTest("Test 14", test_14());
    runTest("Test 15", test_15());
    runTest("Test 16", test_16());
    runTest("Test 17", test_15());
    runTest("Test 18", test_18());
    runTest("Test 19", test_19());
    runTest("Test 20", test_20());
    runTest("Test 21", test_21());
    runTest("Test 22", test_22());
    runTest("Test 23", test_23());
    runTest("Test 24", test_24());
    runTest("Test 25", test_25());
    runTest("Test 26", test_26());
    runTest("Test 27", test_27());
    runTest("Test 28", test_28());
    runTest("Test 29", test_29());
    runTest("Test 30", test_30());
    runTest("Test 32", test_32());
    runTest("Test 33", test_33());
    runTest("Test 34", test_34());
    runTest("Test 35", test_35());
    runTest("Test 36", test_36());
    runTest("Test 37", test_37());
    runTest("Test 38", test_38());

    return 0;
}
