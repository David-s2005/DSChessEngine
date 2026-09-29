#include <iostream>
#include <sstream>
#include <string>

#include "Piece.h"
#include "output.h"
#include "Move_Generator.h"
#include "Evaluation.h"
#include "Search.h"

using std::cout;
using std::endl;

/* TODO:
   Board Heuristics:
   1) Piece square tables
   2) Bishop pair bonus
   3) Pawn structure:
      Punish double pawns (2 on one file)
      Punish single pawns (No pawns on adjacent files)
      Reward passed pawns (Pawns that can reach the end)
      Punish a thin pawn shield (pawns too close to the king)
  4) Reward castling
  5) Reward rooks on open files (reward a touch less for semi open)
  6) Game phase blending

  Search improvement:
  1) [DONE] Killer moves (Moves that triggered beta cutoff)
  2) [DONE] History heuristic (Main a list of frequently used movement patterns)
*/

int main() {
    Board board;
    initPieceArr(board.pieces);
    updateLookupMap(board);

    int index = 0;
    int depth = 4;

    movement moves[218];
    movement playedMoves[1024];
    int consecutiveMoves = 0;
    int noMoves = 0;

    std::string line;

    while(std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if(cmd == "uci") {
            cout << "id name Goliath MK.2" << endl;
            cout << "id author David Smolenaars" << endl;
            cout << "uciok" << endl;
        }
        // Board as been initialized earlier.
        else if(cmd == "isready") {
            cout << "readyok" << endl;
        }
        else if(cmd == "quit") {
            break;
        }
        // Reinitialize board.
        else if(cmd == "ucinewgame") {
            board = Board();
            initKillerMoves();
            initHistory();
            initPieceArr(board.pieces);
            updateLookupMap(board);
            index = 0;
        }
        else if(cmd == "position") {
            std::string next;
            iss >> next;

            if(next == "startpos") {
                board = Board();
                updateLookupMap(board);
                index = 0;
            }

            iss >> next;

            if(next == "moves") {
                std::string moveStr;
                while(iss >> moveStr) {
                    movement move = readMove(moveStr);
                    movePiece(board, move);
                    index++;
                }
            }
        }
        else if(cmd == "go") {
            std::string next;
            iss >> next;

            if(next == "depth") {
                iss >> next; // next stores depth as string.
                Constants::MAX_DEPTH = std::stoi(next);
                initKillerMoves();

                color sideToMove = static_cast<color>(!(index % 2));
                movement AImove = findBestMove(board, sideToMove, Constants::MAX_DEPTH);
                cout << "bestmove " << moveToStr(AImove) << endl;
            }
            else if(next == "movetime") {
                iss >> next; // next stores the time in milliseconds.
                int time = std::stoi(next);
                initKillerMoves();

                color sideToMove = static_cast<color>(!(index % 2));
                movement AImove = iterativeDeepening(board, time, sideToMove);
                cout << "bestmove " << moveToStr(AImove) << endl;
            }
            else {
                color sideToMove = static_cast<color>(!(index % 2));
                movement AImove = findBestMove(board, sideToMove, depth);
                cout << "bestmove " << moveToStr(AImove) << endl;
            }
        }
    }

    return 0;
}
