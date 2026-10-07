#include "Move_Generator.h"

// Returns what's on _position relative to _isWhite. See MoveStatus.
MoveStatus canMoveHere(const Board &_board, const positionRF _position, bool _isWhite) {
    if(_position.rank > Constants::BOARD_SIDE_LEN - 1 ||
       _position.file > Constants::BOARD_SIDE_LEN - 1)
    {
        return OffBoard;
    }

    // Get the piece at _position.
    uint16_t piece_index = _board.indexMap[RFToIndex(_position)];

    if(piece_index == Constants::SENTINEL) {
        return Empty;
    }

    Piece p = _board.pieces[piece_index];

    // May need to reconfigure in future for castling.
    if(isWhite(p) == _isWhite) {
        return OwnPiece;
    }

    return Capture;
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
void generateMovesKnight(uint16_t _piece_index, const Board &_board,
                         movement (&_moves)[], uint16_t &_total_moves)
{
    Piece p = _board.pieces[_piece_index];
    bool white = isWhite(p);
    positionRF piecePos = retPositionRF(p);

    for(int i = 0; i < 8; i++) {
        // Process as int, prevents bound violations we would get with unsigned
        // integers. uint16_t -1 = 0xFFFF.
        int rank = piecePos.rank + knightOffsets[i][0];
        int file = piecePos.file + knightOffsets[i][1];

        if(rank >= 0 && rank <= 7 && file >= 0 && file <= 7) {
            positionRF destination = {static_cast<uint16_t>(rank), static_cast<uint16_t>(file)};

            // We've already checked for correct bounds, so just make sure the tile we are
            // moving into is empty or creates a capture.
            MoveStatus moveStatus = canMoveHere(_board, destination, white);
            if(moveStatus == Empty || moveStatus == Capture) {
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

// Virtually identical to how knight movement works. Offsets are the only difference.
void generateMovesKing(uint16_t _piece_index, const Board &_board,
                       movement (&_moves)[], uint16_t &_total_moves)
{
    Piece p = _board.pieces[_piece_index];
    bool white = isWhite(p);
    positionRF piecePos = retPositionRF(p);

    // If we are checked by a slider, we need to ensure the king cant retreat further into
    // the attack ray / vector. We will get the full ray through boardWithoutKing and see
    // if the king occupies any attacked tiles in that.
    Board boardWithoutKing = _board;
    boardWithoutKing.indexMap[RFToIndex(piecePos)] = Constants::SENTINEL;

    // Check each offset.
    for(int i = 0; i < 8; i++) {
        int rank = piecePos.rank + kingOffsets[i][0];
        int file = piecePos.file + kingOffsets[i][1];

        // Check if advanced position is within the boards bounds.
        if(rank >= 0 && rank <= 7 && file >= 0 && file <= 7) {
            positionRF destination = {static_cast<uint16_t>(rank), static_cast<uint16_t>(file)};

            MoveStatus moveStatus = canMoveHere(_board, destination, white);
            if((moveStatus == Empty || moveStatus == Capture) &&
               isSquareAttacked(boardWithoutKing, destination, static_cast<color>(!white)) == false) {
                _moves[_total_moves] = {piecePos, destination};
                _total_moves++;
            }
        }
    }

    // Add castling moves if allowed.
    if(_board.CastlingRights[BLACKQUEENSIDE] == true &&
       isWhite(p) == BLACK)
    {
        _moves[_total_moves] = {piecePos, {7, C}};
        _total_moves++;
    }
    if(_board.CastlingRights[BLACKKINGSIDE] == true &&
       isWhite(p) == BLACK)
    {
        _moves[_total_moves] = {piecePos, {7, G}};
        _total_moves++;
    }
    if(_board.CastlingRights[WHITEQUEENSIDE] == true &&
       isWhite(p) == WHITE)
    {
        _moves[_total_moves] = {piecePos, {0, C}};
        _total_moves++;
    }
    if(_board.CastlingRights[WHITEKINGSIDE] == true &&
       isWhite(p) == WHITE)
    {
        _moves[_total_moves] = {piecePos, {0, G}};
        _total_moves++;
    }
}

static constexpr int bishopOffsets[4][2] = {
    {-1, 1}, {1, 1}, {1, -1}, {-1, -1}
};

void generateMovesBishop(uint16_t _piece_index, const Board &_board,
                         movement (&_moves)[], uint16_t &_total_moves)
{
    Piece p = _board.pieces[_piece_index];
    bool white = isWhite(p);
    positionRF piecePos = retPositionRF(p);

    // Loop through the 4 offsets
    for(int i = 0; i < 4; i++) {
        positionRF pieceAdvance = piecePos;

        // Shot each ray offset until we've struck a enemy piece, a friendly piece
        // or outside of the board.
        while(true) {
            pieceAdvance.file += static_cast<uint16_t>(bishopOffsets[i][1]);
            pieceAdvance.rank += static_cast<uint16_t>(bishopOffsets[i][0]);
            MoveStatus moveStatus = canMoveHere(_board, pieceAdvance, white);

            // Out of bounds or friendly piece, dont record the last tile of
            // this ray.
            if(moveStatus == OffBoard || moveStatus == OwnPiece) {
                break;
            }

            // Empty or capture. Record the last tile of the ray.
            _moves[_total_moves] = {piecePos, pieceAdvance};
            _total_moves++;

            if(moveStatus == Capture) {
                break;
            }
        }
    }
}

// Rank gets flipped for black pawns. (rank is the 1st element.
static constexpr int pawnOffsets[4][2] = {
    {1, 0}, // Typical move/
    {1, 1}, {1, -1}, // capture offsets.
    {2, 0} // double first move.
};

void generateMovesPawn(uint16_t _piece_index, const Board &_board,
                  movement (&_moves)[], uint16_t &_total_moves)
{
    Piece p = _board.pieces[_piece_index];
    bool white = isWhite(p);
    int rankSign = white ? 1 : -1; // Flip rank if black is playing.
    positionRF piecePos = retPositionRF(p); // Original position.
    positionRF pieceAdvance = piecePos; // Typical 1 rank advance.
    positionRF capRight;
    positionRF capLeft;

    // Capture right.
    capRight.rank = piecePos.rank + rankSign * pawnOffsets[1][0];
    capRight.file = piecePos.file + pawnOffsets[1][1];

    // Capture left.
    capLeft.rank = piecePos.rank + rankSign * pawnOffsets[2][0];
    capLeft.file = piecePos.file + pawnOffsets[2][1];

    // typical move.
    pieceAdvance.rank = piecePos.rank + rankSign * pawnOffsets[0][0];
    pieceAdvance.file = piecePos.file + pawnOffsets[0][1];

    bool isPromotion = false;

    if(white) {
        if(capRight.rank == 7 ||
           capLeft.rank == 7 ||
           pieceAdvance.rank == 7)
        {
            isPromotion = true;
        }
    }
    else {
        if(capRight.rank == 0 ||
           capLeft.rank == 0 ||
           pieceAdvance.rank == 0)
        {
            isPromotion = true;
        }
    }

    // Typical advance is empty.
    if(canMoveHere(_board, pieceAdvance, white) == Empty) {
        if(!isPromotion) {
            _moves[_total_moves] = {piecePos, pieceAdvance};
            _total_moves++;
        }
        // Advance into empty tile leads to promotion.
        else {
            _moves[_total_moves] = {piecePos, pieceAdvance, QUEEN};
            _moves[_total_moves + 1] = {piecePos, pieceAdvance, ROOK};
            _moves[_total_moves + 2] = {piecePos, pieceAdvance, BISHOP};
            _moves[_total_moves + 3] = {piecePos, pieceAdvance, KNIGHT};
            _total_moves += 4;
        }
    }

    // right capture offset goes to opposite color piece. Capture.
    if(canMoveHere(_board, capRight, white) == Capture) {
        if(!isPromotion) {
            _moves[_total_moves] = {piecePos, capRight};
            _total_moves++;
        }
        else {
            _moves[_total_moves] = {piecePos, capRight, QUEEN};
            _moves[_total_moves + 1] = {piecePos, capRight, ROOK};
            _moves[_total_moves + 2] = {piecePos, capRight, BISHOP};
            _moves[_total_moves + 3] = {piecePos, capRight, KNIGHT};
            _total_moves += 4;
        }
    }

    // left capture offset goes to opposite color piece. Capture.
    if(canMoveHere(_board, capLeft, white) == Capture) {
        if(!isPromotion) {
            _moves[_total_moves] = {piecePos, capLeft};
            _total_moves++;
        }
        else {
            _moves[_total_moves] = {piecePos, capLeft, QUEEN};
            _moves[_total_moves + 1] = {piecePos, capLeft, ROOK};
            _moves[_total_moves + 2] = {piecePos, capLeft, BISHOP};
            _moves[_total_moves + 3] = {piecePos, capLeft, KNIGHT};
            _total_moves += 4;
        }
    }

    // right hand en passant attack.
    if(capRight.rank == _board.EnPassantTarget.rank &&
       capRight.file == _board.EnPassantTarget.file)
    {
        _moves[_total_moves] = {piecePos, capRight};
        _total_moves++;
    }

    // left hand en passant attack.
    if(capLeft.rank == _board.EnPassantTarget.rank &&
       capLeft.file == _board.EnPassantTarget.file)
    {
        _moves[_total_moves] = {piecePos, capLeft};
        _total_moves++;
    }

    // Double jump first move. Check if we are leaping over anything
    // before continuing.
    positionRF intermediate;
    intermediate.rank = piecePos.rank + rankSign * pawnOffsets[0][0];
    intermediate.file = piecePos.file + pawnOffsets[0][1];

    pieceAdvance.rank = piecePos.rank + rankSign * pawnOffsets[3][0];
    pieceAdvance.file = piecePos.file + pawnOffsets[3][1];

    if(canMoveHere(_board, intermediate, white) == Empty &&
       canMoveHere(_board, pieceAdvance, white) == Empty &&
       hasMoved(p) == false)
    {
        _moves[_total_moves] = {piecePos, pieceAdvance};
        _total_moves++;
    }
}

static constexpr int rookOffsets[4][2] = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}
};

void generateMovesRook(uint16_t _piece_index, const Board &_board,
                       movement (&_moves)[], uint16_t &_total_moves)
{
    Piece p = _board.pieces[_piece_index];
    bool white = isWhite(p);
    positionRF piecePos = retPositionRF(p);
    positionRF pieceAdvance;

    // Loop through the 4 offsets
    for(int i = 0; i < 4; i++) {
        pieceAdvance = piecePos;

        while(true) {
            pieceAdvance.file += static_cast<uint16_t>(rookOffsets[i][1]);
            pieceAdvance.rank += static_cast<uint16_t>(rookOffsets[i][0]);
            MoveStatus moveStatus = canMoveHere(_board, pieceAdvance, white);

            // Next tile is either outside the board or a piece of the same color.
            // end without recording this advance.
            if(moveStatus == OffBoard || moveStatus == OwnPiece) {
                break;
            }

            // Next tile can be attacked, record & quit.
            if(moveStatus == Capture) {
                _moves[_total_moves] = {piecePos, pieceAdvance};
                _total_moves++;

                break;
            }

            // Next tile is empty, record and move to next tile.
            if(moveStatus == Empty) {
                _moves[_total_moves] = {piecePos, pieceAdvance};
                _total_moves++;
            }
        }
    }
}

static constexpr int queenOffsets[8][2] = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // Rook style offsets.
    {-1, 1}, {1, 1}, {1, -1}, {-1, -1} // Queen style offsets.
};

void generateMovesQueen(uint16_t _piece_index, const Board &_board,
                       movement (&_moves)[], uint16_t &_total_moves)
{
    Piece p = _board.pieces[_piece_index];
    bool white = isWhite(p);
    positionRF piecePos = retPositionRF(p);
    positionRF pieceAdvance;

    // Loop through the 4 offsets
    for(int i = 0; i < 8; i++) {
        pieceAdvance = piecePos;

        while(true) {
            pieceAdvance.file += static_cast<uint16_t>(queenOffsets[i][1]);
            pieceAdvance.rank += static_cast<uint16_t>(queenOffsets[i][0]);
            MoveStatus moveStatus = canMoveHere(_board, pieceAdvance, white);

            // Next tile is either outside the board or a piece of the same color.
            // end without recording this advance.
            if(moveStatus == OffBoard || moveStatus == OwnPiece) {
                break;
            }

            // Next tile can be attacked, record & quit.
            if(moveStatus == Capture) {
                _moves[_total_moves] = {piecePos, pieceAdvance};
                _total_moves++;

                break;
            }

            // Next tile is empty, record and move to next tile.
            if(moveStatus == Empty) {
                _moves[_total_moves] = {piecePos, pieceAdvance};
                _total_moves++;
            }
        }
    }
}

// Returns the attacked tiles a given pawn has.
void retAttackPawn(const Board &_board, positionRF _pawnPosition, movement (&_attacks)[2]) {
    Piece pawn = _board.pieces[_board.indexMap[RFToIndex(_pawnPosition)]];
    int rankSign = isWhite(pawn) ? 1 : -1;

    for(int i = 0; i < 2; i++) {
        _attacks[i].start = _pawnPosition;

        int rank = _pawnPosition.rank + rankSign * pawnOffsets[i + 1][0];
        int file = _pawnPosition.file + pawnOffsets[i + 1][1];

        if(rank >= 0 && rank <= 7 && file >= 0 && file <= 7) {
            _attacks[i].end = {static_cast<uint16_t>(rank), static_cast<uint16_t>(file)};
        }
        else {
            _attacks[i].end = {Constants::SENTINEL, Constants::SENTINEL};
        }
    }
}

// Returns the attacked tiles a king has.
void retAttackKing(positionRF _kingPos, movement (&_attacks)[8]) {
    for(int i = 0; i < 8; i++) {
        _attacks[i].start = _kingPos;
        int rank = _kingPos.rank + kingOffsets[i][0];
        int file = _kingPos.file + kingOffsets[i][1];

        if(rank >= 0 &&
           rank <= 7 &&
           file >= 0 &&
           file <= 7)
        {
            _attacks[i].end.rank = static_cast<uint16_t>(rank);
            _attacks[i].end.file = static_cast<uint16_t>(file);
        }
        else {
            _attacks[i].end = {Constants::SENTINEL, Constants::SENTINEL};
        }
    }
}

// Loop through every piece of the attacking color and determine if any of the returned
// movements end tiles are the same as _position.
bool isSquareAttacked(const Board &_board, positionRF _position, color _attacking_color) {
    bool attacked = false;

    for(int i = 0; i < Constants::NO_PIECES; i++) {
        Piece p = _board.pieces[i];

        if(inPlay(p) == true && isWhite(p) == _attacking_color) {
            if(attacked == true) return true;

            positionRF pos = retPositionRF(p);
            uint16_t pos_index = RFToIndex(pos);
            PieceType type = retType(p);

            switch(type) {
            case PAWN: {
                movement moves[2];
                retAttackPawn(_board, pos, moves);
                attacked = containsPosition(_position, moves, 2);

                break;
            }

            case KING: {
                movement moves[8];
                retAttackKing(pos, moves);
                attacked = containsPosition(_position, moves, 8);

                break;
            }

            case QUEEN: {
                movement moves[Constants::MAX_ATTACKS];
                uint16_t no_moves = 0;
                generateMovesQueen(i, _board, moves, no_moves);
                attacked = containsPosition(_position, moves, no_moves);

                break;
            }

            case ROOK: {
                movement moves[Constants::MAX_ATTACKS];
                uint16_t no_moves = 0;
                generateMovesRook(i, _board, moves, no_moves);
                attacked = containsPosition(_position, moves, no_moves);

                break;
            }

            case BISHOP: {
                movement moves[Constants::MAX_ATTACKS];
                uint16_t no_moves = 0;
                generateMovesBishop(i, _board, moves, no_moves);
                attacked = containsPosition(_position, moves, no_moves);

                break;
            }

            case KNIGHT: {
                movement moves[Constants::MAX_ATTACKS];
                uint16_t no_moves = 0;
                generateMovesKnight(i, _board, moves, no_moves);
                attacked = containsPosition(_position, moves, no_moves);

                break;
            }

            default: // Shouldn't execute at all. Maybe cause a hard crash?
                break;
            }
        }
    }
    return attacked;
}

// Checks if _pos is present as a end tile in moves.
bool containsPosition(positionRF _pos, const movement *_moves, uint16_t _noMoves) {
    for(uint16_t i = 0; i < _noMoves; i++) {
        if(_moves[i].end.rank == _pos.rank &&
           _moves[i].end.file == _pos.file)
        {
            return true;
        }
    }
    return false;
}

void movePiece(Board &_board, movement _move) {
    positionRF start = _move.start;
    positionRF end = _move.end;
    uint16_t start_index = RFToIndex(start);
    uint16_t end_index = RFToIndex(end);
    Piece &moving_piece = _board.pieces[_board.indexMap[start_index]];
    bool white = isWhite(moving_piece);
    bool isPawn = retType(moving_piece) == PAWN;
    bool newEnPassantTarget = false;

    if(_move.promotionType == NONE) {
        // Capture. Disable the target tiles piece.
        uint16_t captured_index = _board.indexMap[end_index];
        if(captured_index != Constants::SENTINEL) {
            Piece &capture_piece = _board.pieces[captured_index];
            setPlay(capture_piece, false); // Target piece has been capped.
        }

        // Castle. Figure out if our king is moving more than one file at a time.
        if(retType(moving_piece) == KING &&
           abs(static_cast<int>(start.file) - static_cast<int>(end.file)) > 1)
        {
            if(isWhite(moving_piece) == BLACK) {
                // black kingside castle.
                if((static_cast<int>(start.file) - static_cast<int>(end.file)) < 0) {
                    // Move the rook.
                    Piece &rook = _board.pieces[_board.indexMap[RFToIndex({7, H})]];
                    setPositionRF(7, F, rook);
                    setMoved(rook, true);
                }
                // black queenside castle.
                if((static_cast<int>(start.file) - static_cast<int>(end.file)) > 0) {
                    Piece &rook = _board.pieces[_board.indexMap[RFToIndex({7, A})]];
                    setPositionRF(7, D, rook);
                    setMoved(rook, true);
                }
            }
            if(isWhite(moving_piece) == WHITE) {
                // white kingside castle.
                if((static_cast<int>(start.file) - static_cast<int>(end.file)) < 0) {
                    Piece &rook = _board.pieces[_board.indexMap[RFToIndex({0, H})]];
                    setPositionRF(0, F, rook);
                    setMoved(rook, true);
                }
                // white queenside castle.
                if((static_cast<int>(start.file) - static_cast<int>(end.file)) > 0) {
                    Piece &rook = _board.pieces[_board.indexMap[RFToIndex({0, A})]];
                    setPositionRF(0, D, rook);
                    setMoved(rook, true);
                }
            }

            // Move the king.
            setPositionRF(end.rank, end.file, moving_piece);
        }

        // en passant attack. The destination is the passed-over target
        // square, not the square the captured pawn is actually sitting on.
        if(isPawn &&
           end.rank == _board.EnPassantTarget.rank &&
           end.file == _board.EnPassantTarget.file)
        {
            positionRF attackedPawnRF = _board.EnPassantTarget;

            if(white) {
                // Pawn being en passant'ed is directly below the en passant target tile
                // if the attacking pawn is white.
                attackedPawnRF.rank -= 1;
            }
            // Otherwise, the pawn being attacked is directly above the en passant target square.
            else {
                attackedPawnRF.rank += 1;
            }

            uint16_t attacked_index = _board.indexMap[RFToIndex(attackedPawnRF)];
            if(attacked_index != Constants::SENTINEL) {
                setPlay(_board.pieces[attacked_index], false);
            }
        }

        // double jump pawn. New en passant target created.
        if(isPawn &&
           abs(static_cast<int>(start.rank) - static_cast<int>(end.rank)) > 1) {

           if(white) {
                _board.EnPassantTarget.rank = end.rank - 1;
           }
           else _board.EnPassantTarget.rank = end.rank + 1;

           _board.EnPassantTarget.file = end.file;
           newEnPassantTarget = true;
        }

        setPositionRF(end.rank, end.file, moving_piece);
    }
    // Piece promotion.
    else {
        if(end.rank != start.rank || end.file != start.file) {
            uint16_t captured_index = _board.indexMap[end_index];
            // Capture leads to promotion.
            if(captured_index != Constants::SENTINEL) {
                Piece &capture_piece = _board.pieces[captured_index];
                setPlay(capture_piece, false);
            }
            setPositionRF(end.rank, end.file, moving_piece);
        }
        setType(_move.promotionType, moving_piece);
    }

    if(!newEnPassantTarget) {
        _board.EnPassantTarget = {Constants::SENTINEL, Constants::SENTINEL};
    }

    setMoved(moving_piece, true);
    updateLookupMap(_board);

    // No good heuristic for determining if we need to update castling rights or
    // not. Recompute.
    updateCastlingRights(_board);

    // Flip white. Next colors turn.
    _board.moving = static_cast<color>(!white);
}

void updateCastlingRights(Board &_board) {
    positionRF blackQueensideEmpty[] = {{7, D}, {7, C}, {7, B}};
    positionRF blackQueensideSafe[] = {{7,E}, {7,D}, {7,C}};

    positionRF whiteQueensideEmpty[] = {{0, D}, {0, C}, {0, B}};
    positionRF whiteQueensideSafe[] = {{0,E}, {0,D}, {0,C}};

    positionRF blackKingsideEmpty[] = {{7, F}, {7, G}};
    positionRF blackKingsideSafe[] = {{7, E}, {7, F}, {7, G}};

    positionRF whiteKingsideEmpty[] = {{0, F}, {0, G}};
    positionRF whiteKingsideSafe[] = {{0, E}, {0, F}, {0, G}};

    _board.CastlingRights[BLACKQUEENSIDE] = canCastle(_board, {7, E}, {7, A},
                                                      blackQueensideEmpty,
                                                      3, blackQueensideSafe,
                                                      3, WHITE);

    _board.CastlingRights[BLACKKINGSIDE] = canCastle(_board, {7, E}, {7, H},
                                                      blackKingsideEmpty,
                                                      2, blackKingsideSafe,
                                                      3, WHITE);

    _board.CastlingRights[WHITEQUEENSIDE] = canCastle(_board, {0, E}, {0, A},
                                                      whiteQueensideEmpty,
                                                      3, whiteQueensideSafe,
                                                      3, BLACK);

    _board.CastlingRights[WHITEKINGSIDE] = canCastle(_board, {0, E}, {0, H},
                                                      whiteKingsideEmpty,
                                                      2, whiteKingsideSafe,
                                                      3, BLACK);
}

// Helper function for updateCastlingRights. Validates that a castle can
// occur given the parameters.
bool canCastle(const Board &_board, positionRF _kingPos, positionRF _rookPos,
               const positionRF *_emptySquares, int _noEmptySquares,
               const positionRF *_safeSquares, int _noSafeSquares,
               color _attackingColor)
{
    // Prevent segfault by checking if the starting positions of the king
    // & rook are empty. Cant decode a empty tile, junk data.
    if(_board.indexMap[RFToIndex(_kingPos)] == Constants::SENTINEL ||
       _board.indexMap[RFToIndex(_rookPos)] == Constants::SENTINEL)
    {
        return false;
    }

    // Has either piece moved yet? No need to check the colors of the pieces
    // as a white king cant castle with a black rook because the black rook
    // has moved, thus invalidating the move.
    if(hasMoved(_board.pieces[_board.indexMap[RFToIndex(_kingPos)]]) ||
       hasMoved(_board.pieces[_board.indexMap[RFToIndex(_rookPos)]]))
    {
        return false;
    }

    // Run through what should be empty spaces between the rook and king.
    for(int i = 0; i < _noEmptySquares; i++) {
        if(_board.indexMap[RFToIndex(_emptySquares[i])] != Constants::SENTINEL) return false;
    }

    // Make sure the king isn't checked, the interim tile is not attacked & that the
    // end position of the king isn't attacked (don't move into check.)
    for(int i = 0; i < _noSafeSquares; i++) {
        if(isSquareAttacked(_board, _safeSquares[i], _attackingColor)) return false;
    }

    return true;
}

// Finds the position of the king of the given color on _board.
positionRF findKingPos(const Board &_board, color _isWhite) {
    positionRF kingPos = {Constants::SENTINEL, Constants::SENTINEL};

    for(int i = 0; i < Constants::NO_TILES; i++) {
        if(_board.indexMap[i] != Constants::SENTINEL) {
            Piece rp = _board.pieces[_board.indexMap[i]];
            if(retType(rp) == KING && isWhite(rp) == _isWhite) {
                kingPos = retPositionRF(rp);
            }
        }
    }

    return kingPos;
}

void generateMovesForSide(const Board &_board, color _isWhite, movement (&_moves)[],
                          uint16_t _total_moves, movement (&_legal_moves)[],
                          uint16_t &_total_legal_moves)
{
    Board testBoard = _board;
    updateLookupMap(testBoard);

    positionRF kingPos = findKingPos(_board, _isWhite);

    for(int i = 0; i < Constants::NO_TILES; i++) {
        Piece p;
        if(_board.indexMap[i] != Constants::SENTINEL) {
            p = _board.pieces[_board.indexMap[i]];
        }
        else continue;

        if(isWhite(p) == _isWhite && inPlay(p) == true) {
            PieceType type = retType(p);
            uint16_t piece_index = _board.indexMap[i];

            switch(type) {
                case QUEEN : {
                    generateMovesQueen(piece_index, _board, _moves, _total_moves);
                    break;
                }
                case ROOK : {
                    generateMovesRook(piece_index, _board, _moves, _total_moves);
                    break;
                }
                case BISHOP : {
                    generateMovesBishop(piece_index, _board, _moves, _total_moves);
                    break;
                }
                case KNIGHT : {
                    generateMovesKnight(piece_index, _board, _moves, _total_moves);
                    break;
                }
                case PAWN : {
                    generateMovesPawn(piece_index, _board, _moves, _total_moves);
                    break;
                }
                case KING : {
                    generateMovesKing(piece_index, _board, _moves, _total_moves);
                    break;
                }
                default:
                    break;
            }
        }
    }

    // King is in check. Only valid moves are the ones that takes the king out
    // of check.
    if(isSquareAttacked(testBoard, kingPos, static_cast<color>(!_isWhite)) == true) {
        for(int i = 0; i < _total_moves; i++) {
            movePiece(testBoard, _moves[i]);
            positionRF newKingPos = findKingPos(testBoard, _isWhite);

            // This move takes the king out of check.
            if(isSquareAttacked(testBoard, newKingPos, static_cast<color>(!_isWhite)) == false) {
                _legal_moves[_total_legal_moves] = _moves[i];
                _total_legal_moves++;
            }

            testBoard = _board;
        }
    }
    else {
        for(int i = 0; i < _total_moves; i++) {
            movePiece(testBoard, _moves[i]);

            positionRF newKingPos = findKingPos(testBoard, _isWhite);

            if(isSquareAttacked(testBoard, newKingPos, static_cast<color>(!_isWhite)) != true) {
                _legal_moves[_total_legal_moves] = _moves[i];
                _total_legal_moves++;
            }

            testBoard = _board;
        }
    }
}

// Wrapper for generateMovesForSide.
void generateLegalMoves(const Board &_board, color _isWhite,
                        movement (&_moves)[], int &_total_moves)
{
    movement pseudoLegalMoves[218];
    uint16_t noPseudoLegalMoves = 0;
    uint16_t noLegalMoves = 0;

    generateMovesForSide(_board, _isWhite, pseudoLegalMoves,
                         noPseudoLegalMoves, _moves, noLegalMoves);

    _total_moves = noLegalMoves;
}

bool isLegalMove(movement _move, const movement (&_moves)[], int _noMoves) {
    for(int i = 0; i < _noMoves; i++) {
        if(_move.start.rank == _moves[i].start.rank &&
           _move.start.file == _moves[i].start.file &&
           _move.end.rank == _moves[i].end.rank &&
           _move.end.file == _moves[i].end.file)
        {
            return true;
        }
    }

    return false;
}

