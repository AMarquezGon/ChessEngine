#include "Board.h"
#include "settings.h"

using BitBoard = std::uint64_t;

Board::Board()
{
	m_pieces[pawn + max_pieces * white] = constants::RANK_2;
	m_pieces[pawn + max_pieces * black] = constants::RANK_6;

	m_pieces[knight + max_pieces * white] = constants::RANK_1 & (constants::FILE_B | constants::FILE_G) ;
	m_pieces[knight + max_pieces * black] = constants::RANK_7 & (constants::FILE_B | constants::FILE_G);

	m_pieces[bishop + max_pieces * white] = constants::RANK_1 & (constants::FILE_C | constants::FILE_F);
	m_pieces[bishop + max_pieces * black] = constants::RANK_7 & (constants::FILE_C | constants::FILE_F);

	m_pieces[rook + max_pieces * white] = constants::RANK_1 & (constants::FILE_A | constants::FILE_H);
	m_pieces[rook + max_pieces * black] = constants::RANK_7 & (constants::FILE_A | constants::FILE_H);

	m_pieces[queen + max_pieces * white] = constants::RANK_1 & constants::FILE_D;
	m_pieces[queen + max_pieces * black] = constants::RANK_7 & constants::FILE_D;

	m_pieces[king + max_pieces * white] = constants::RANK_1 & constants::FILE_E;
	m_pieces[king + max_pieces * black] = constants::RANK_7 & constants::FILE_E;
}

void Board::setBit(Piece piece,Color color, int file, int rank)
{
	m_pieces[piece + max_pieces * color] |= (1ULL << ((file-1) + (rank - 1) * 8));
}

bool Board::getBit(Piece piece, Color color, int file, int rank)
{
	return m_pieces[piece + max_pieces * color] & (1ULL << ((file - 1) + (rank - 1) * 8));
}

void Board::clearBit(Piece piece, Color color, int file, int rank)
{
	m_pieces[piece + max_pieces * color] &= ~(1ULL << ((file - 1) + (rank - 1) * 8));
}