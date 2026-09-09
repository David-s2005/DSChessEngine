#include <iostream>

#include "Piece.h"
#include "output.h"

using std::cout;
using std::endl;

/* TODO

*/

int main() {
    Board board;

    initPieceArr(board.pieces);
    updateLookupMap(board);

    string FEN = RetFEN(board, true);

    cout << FEN << endl;
    return 0;
}
