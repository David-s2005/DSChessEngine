#include <iostream>

#include "Piece.h"
#include "output.h"

using std::cout;
using std::endl;

/* TODO
   Create and maintain a lookup table for piece-to-positions:

   [-1, -1,  5, ..., -1] <- each element represents a tile.
   [-1, -1, -1, ..., -1]
   [ 2,  3,  1, -1,  -1]

   arr[3] = 5
   pieces[5] -> Piece x.
*/

int main() {
    Piece Pieces[32];

    initPieceArr(Pieces);

    return 0;
}
