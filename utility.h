#pragma once
#include "Board.h"

constexpr std::size_t toIndex(Board::Color c) {
    return (c == Board::white) ? 0 : 1;
}

constexpr std::size_t toOtherIndex(Board::Color c) {
    return (c == Board::white) ? 1 : 0;
}

constexpr Board::Color oppositeColor(Board::Color color)
{
    return color == Board::white ? Board::black : Board::white;
}

std::uint64_t shift(std::uint64_t value, std::uint8_t shift, Board::Color color)
{
    return color == Board::white ? value << shift : value >> shift;
}

std::uint64_t enpassantCheckRight(std::uint64_t value, Board::Color color)
{
    return color == Board::white ? value >> 31 : value >> 23;
}

std::uint64_t enpassantCheckLeft(std::uint64_t value, Board::Color color)
{
    return color == Board::white ? value >> 33 : value >> 25;
}
//Piece moves
//Bit functions (move out of class)
//Board representation
