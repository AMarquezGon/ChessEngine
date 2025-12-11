#include "Board.h"

using BitBoard = std::uint64_t;

void Board::setBit(Piece piece,Color color, int file, int rank)
{
	m_pieces[piece + max_pieces * color] |= (1ULL << (file + (rank - 1) * 8));
}

BitBoard Board::getBit(Piece piece, Color color, int file, int rank)
{
	return m_pieces[piece + max_pieces * color] & (1ULL << (file + (rank - 1) * 8));
}

void Board::clearBit(Piece piece, Color color, int file, int rank)
{
	m_pieces[piece + max_pieces * color] &= ~(1ULL << (file + (rank - 1) * 8));
}