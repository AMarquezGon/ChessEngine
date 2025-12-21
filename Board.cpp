#include "Board.h"
#include "utility.h"

using BitBoard = std::uint64_t;

Board::Board()
{
	m_currentTurn = white;
	m_castling = 3;

	m_pieces[pawn + max_pieces * white] = constants::RANK_2;
	m_pieces[pawn + max_pieces * black] = constants::RANK_7;

	m_pieces[knight + max_pieces * white] = constants::RANK_1 & (constants::FILE_B | constants::FILE_G) ;
	m_pieces[knight + max_pieces * black] = constants::RANK_8 & (constants::FILE_B | constants::FILE_G);

	m_pieces[bishop + max_pieces * white] = constants::RANK_1 & (constants::FILE_C | constants::FILE_F);
	m_pieces[bishop + max_pieces * black] = constants::RANK_8 & (constants::FILE_C | constants::FILE_F);

	m_pieces[rook + max_pieces * white] = constants::RANK_1 & (constants::FILE_A | constants::FILE_H);
	m_pieces[rook + max_pieces * black] = constants::RANK_8 & (constants::FILE_A | constants::FILE_H);

	m_pieces[queen + max_pieces * white] = constants::RANK_1 & constants::FILE_D;
	m_pieces[queen + max_pieces * black] = constants::RANK_8 & constants::FILE_D;

	m_pieces[king + max_pieces * white] = constants::RANK_1 & constants::FILE_E;
	m_pieces[king + max_pieces * black] = constants::RANK_8 & constants::FILE_E;
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

BitBoard Board::getPieces(Color color)
{
	return { m_pieces[pawn + max_pieces * color] | m_pieces[knight + max_pieces * color] | m_pieces[bishop + max_pieces * color] | m_pieces[rook + max_pieces * color] | m_pieces[king + max_pieces * color] | m_pieces[queen + max_pieces * color] };
}

void Board::display()
{
	for (int rank{ 7 }; rank > -1; --rank)
	{
		for (int file{ 0 }; file < 8; ++file)
		{
			bool found{ false };
			int piece{};
			for (const auto& map : m_pieces)
			{
				if (1ULL << (file + rank * 8) & map)
				{
					std::cout << indexToChar[piece] << ' ';
					found = true;
					break;
				}
				++piece;
			}
			if (!found)
				std::cout << "0 ";
		}
		std::cout << '\n';
	}
}

void Board::eliminatePiece(BitBoard position)
{
	for (auto& map : m_pieces)
	{
		map &= (~position);
	}
}

bool Board::operator==(const Board& other) const
{
	for (std::size_t i{ 0 }; i < 12; ++i)
	{
		if (m_pieces[i] != other.m_pieces[i])
			return false;
	}
	return m_castling == other.m_castling && m_enpassant == other.m_enpassant && m_currentTurn == other.m_currentTurn;
}