#pragma once
#include "Board.h"

constexpr std::size_t toIndex(Board::Color c) {
    return (c == Board::white) ? 0 : 1;
}

constexpr std::size_t toOtherIndex(Board::Color c) {
    return (c == Board::white) ? 1 : 0;
}
//Piece moves
//Bit functions (move out of class)
//Board representation