#include <iostream>

#include "Piece.h"
#include "Bitwise_Logic.h"

using std::cout;
using std::endl;

// Ensure that isWhite returns a valid value for a black & white piece.
bool test_1() {
    Piece Queen1 = 0x0017; // In play, white (bit 1 set), type=QUEEN, position A1.
    Piece Queen2 = 0x0015; // In play, black (bit 1 clear), type=QUEEN, position A1.

    return isWhite(Queen1) && !(isWhite(Queen2));
}

// Ensure that inPlay returns a valid value for a piece that is on
// and off the board.
bool test_2() {
    Piece onBoard = 0x0001;  // Bit 0 set.
    Piece offBoard = 0x0000; // Bit 0 clear.

    return inPlay(onBoard) && !inPlay(offBoard);
}

// Ensure that retSequence extracts the correct bits from a sequence.
bool test_3() {
    uint16_t sequenceA = 0x00F0; // 0000 0000 1111 0000
    uint16_t sequenceB = 0x0034; // 0000 0000 0011 0100

    return retSequence(sequenceA, 4, 4) == 0xF &&
           retSequence(sequenceB, 2, 3) == 0x5;
}

// Ensure that retPosition extracts the 6 position bits (5-10) of a piece,
// covering the full 0-63 range of an 8x8 board.
bool test_4() {
    Piece posLow = 0x00A0;  // Position bits (5-10) = 000101 (5).
    Piece posMax = 0x07E0;  // Position bits (5-10) = 111111 (63).

    return retPosition(posLow) == 5 && retPosition(posMax) == 63;
}

// Ensure that setSequence inserts a bit sequence of arbitrary length
// at an arbitrary position without disturbing the surrounding bits.
bool test_5() {
    uint16_t sequenceA = 0x0000; // 0000 0000 0000 0000
    uint16_t sequenceB = 0x00FF; // 0000 0000 1111 1111

    setSequence(sequenceA, 4, 4, 0xF);
    setSequence(sequenceB, 2, 3, 0x5);

    return sequenceA == 0x00F0 && sequenceB == 0x00F7;
}

// Ensure that setPositionRF/retPositionRF round-trip a rank and file
// consistently (pos = rank * 8 + file on both the encode and decode
// side), and that a piece's other bits are left untouched.
bool test_6() {
    Piece piece = 0x0001; // In play (bit 0), black (bit 1 clear).

    setPositionRF(3, 5, piece);
    positionRF result = retPositionRF(piece);

    return result.rank == 3 && result.file == 5 &&
           inPlay(piece) && !isWhite(piece);
}

// Ensure that setType/retType read and write the 3 type bits (2-4)
// without disturbing the piece's other bits.
bool test_7() {
    Piece piece = 0x0001; // In play, black (bit 1 clear).

    setType(QUEEN, piece);

    return retType(piece) == QUEEN && inPlay(piece) && !isWhite(piece);
}

// Ensure that setID/retID read and write the 4 ID bits (11-14)
// without disturbing the piece's other bits.
bool test_8() {
    Piece piece = 0x0001; // In play, black (bit 1 clear).

    setID(0b1110, piece); // Queen's ID.

    return retID(piece) == 0b1110 && inPlay(piece) && !isWhite(piece);
}

// Ensure that setMoved/hasMoved read and write the has-moved flag
// (bit 15) without disturbing the piece's other bits.
bool test_9() {
    Piece piece = 0x0001; // In play, black (bit 1 clear).

    setMoved(piece, true);

    return hasMoved(piece) && inPlay(piece) && !isWhite(piece);
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
}
