#include <iostream>

#include "Piece.h"
#include "Bitwise_Logic.h"
#include "Board.h"
#include "output.h"
#include "Constants.h"
#include "Move_Generator.h"

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

// updateLookupMap test.
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

    showBoard(b);

    return true;
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

    return 0;
}
