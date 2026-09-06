#include <iostream>

#include "Piece.h"

using std::cout;
using std::endl;

int main()
{
    Piece Queen = 0x0000000000111001; // White queen at A1.

    if(isWhite(Queen)) {
        cout << "Queen is white!" << endl;
    }
    else {
        cout << "Queen isn't white!" << endl;
    }

    return 0;
}
