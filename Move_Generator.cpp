#include "Move_Generator.h"

// Returns a int value depending on what can be done on this tile. 0 = cant move here,
// 1 = can move here because its empty & 2 = Can move here via attack.
uint16_t canMoveHere(const Board &_board, const positionRF _position, bool _isWhite) {
    // Target is outside the board. code 0.
    if(_position.rank > Constants::BOARD_SIDE_LEN ||
       _position.file > Constants::BOARD_SIDE_LEN)
    {
        return 0;
    }

    // Get the piece at _position.
    uint16_t piece_index = _board.indexMap[RFToIndex(_position)];

    // Empty square. code 2.
    if(piece_index == Constants::SENTINEL) {
        return 2;
    }

    Piece p = _board.pieces[piece_index];

    // Same color. Code 1. Cant move here. May need to
    // reconfigure in future for castling.
    if(isWhite(p) == _isWhite) {
        return 1;
    }

    // Opposite color in target tile. This piece can get attacked.
    return 3;
}

// Moves knight. _piece_index is the index of the thing we're trying to move.
// _board is the board state. _moves is a array of what actions we can make here.
// & total moves is the length of _moves.
static constexpr int knightOffsets[8][2] = {
    {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
    { 1, -2}, { 1, 2}, { 2, -1}, { 2, 1}
};

// No need to validate type here. This will be handled by a different function.

// Places the possible moves a given knight can make from the position at
// _piece_index into _moves.
void generateMovesKnight(uint16_t _piece_index, Board &_board,
                         movement (&_moves)[], uint16_t &_total_moves)
{
    Piece p = _board.pieces[_piece_index];
    positionRF piecePos = retPositionRF(p);

    for(int i = 0; i < 8; i++) {
        // Process as int, prevents bound violations we would get with unsigned
        // integers. uint16_t -1 = 0xFFFF.
        int rank = piecePos.rank + knightOffsets[i][0];
        int file = piecePos.file + knightOffsets[i][1];

        if(rank >= 0 && rank <= 7 && file >= 0 && file <= 7) {
            positionRF destination = {static_cast<uint16_t>(rank), static_cast<uint16_t>(file)};

            // 2 = empty, 3 = capture. 0 (bounds) and 1 (same-color) are not
            // reachable here since (rank, file) is already bounds-checked
            // above, but excluding them explicitly keeps this correct
            // regardless of canMoveHere's exact code values.
            uint16_t moveStatus = canMoveHere(_board, destination, isWhite(p));
            if(moveStatus == 2 || moveStatus == 3) {
                _moves[_total_moves] = {piecePos, destination};
                _total_moves++;
            }
        }
    }
}

static constexpr int kingOffsets[8][2] = {
    {-1, 1}, {0, 1}, {1, 1},
    {-1, 0},         {1, 0},
    {-1,-1}, {0,-1}, {1,-1}
};

// This needs more work. I need a way to determine if the king will be in check if it goes
// into a certain tile.

// Virtually identical to how knight movement works. Offsets are the only difference.
void generateMovesKing(uint16_t _piece_index, Board &_board,
                       movement (&_moves)[], uint16_t &_total_moves)
{
    Piece p = _board.pieces[_piece_index];
    positionRF piecePos = retPositionRF(p);

    for(int i = 0; i < 8; i++) {
        int rank = piecePos.rank + kingOffsets[i][0];
        int file = piecePos.file + kingOffsets[i][1];

        if(rank >= 0 && rank <= 7 && file >= 0 && file <= 7) {
            positionRF destination = {static_cast<uint16_t>(rank), static_cast<uint16_t>(file)};

            // 2 = empty, 3 = capture.
            uint16_t moveStatus = canMoveHere(_board, destination, isWhite(p));
            if(moveStatus == 2 || moveStatus == 3) {
                _moves[_total_moves] = {piecePos, destination};
                _total_moves++;
            }
        }
    }
}

static constexpr int bishopOffsets[4][2] = {
    {-1, 1}, {1, 1}, {1, -1}, {-1, -1}
};

void generateMovesBishop(uint16_t _piece_index, Board &_board,
                         movement (&_moves)[], uint16_t &_total_moves)
{
    Piece p = _board.pieces[_piece_index];
    positionRF piecePos = retPositionRF(p);

    // Loop through the 4 offsets
    for(int i = 0; i < 4; i++) {
        // Each direction's ray starts fresh from the piece's own square.
        positionRF pieceAdvance = piecePos;

        // Iterate through this offset until we find a piece we can attack,
        // a tile occupied by another piece of the same color, or we've reached
        // outside the bounds of the board.
        while(true) {
            pieceAdvance.file += static_cast<uint16_t>(bishopOffsets[i][1]);
            pieceAdvance.rank += static_cast<uint16_t>(bishopOffsets[i][0]);
            uint16_t moveStatus = canMoveHere(_board, pieceAdvance, isWhite(p));

            // 0 = off the board, 1 = blocked by our own piece. Stop the ray
            // without recording this square either way.
            if(moveStatus == 0 || moveStatus == 1) {
                break;
            }

            // 2 = empty (record and keep sliding), 3 = capture (record and
            // stop.
            _moves[_total_moves] = {piecePos, pieceAdvance};
            _total_moves++;

            if(moveStatus == 3) {
                break;
            }
        }
    }
}







