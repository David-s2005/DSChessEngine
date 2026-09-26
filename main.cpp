#include <iostream>

#include "Piece.h"
#include "output.h"
#include "Move_Generator.h"

using std::cout;
using std::endl;

int main() {
    Board board;

    initPieceArr(board.pieces);
    updateLookupMap(board);

    string input;
    int index = 0;

    while(true) {
        showBoard(board);
        cout << "Enter your move: " << endl;
        std::cin >> input;
        movement move = readMove(input);
        movement moves[218];
        int noMoves = 0;

        if(move.start.rank == Constants::SENTINEL) break;
        if(move.start.file == Constants::SENTINEL) break;
        if(move.end.rank == Constants::SENTINEL) break;
        if(move.end.file == Constants::SENTINEL) break;

        generateLegalMoves(board, static_cast<color>(!(index % 2)),
                           moves, noMoves);

        if(isLegalMove(move, moves, noMoves)) {
            movePiece(board, move);
        }
        else cout << "Illegal move!" << endl;

        index++;
    }

    return 0;
}
