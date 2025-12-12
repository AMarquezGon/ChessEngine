#include "Board.h"
#include "settings.h"
#include <iostream>
#include <bitset>

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

BitBoard Board::getPieces(Color color)
{
	return { m_pieces[pawn + max_pieces * color] | m_pieces[knight + max_pieces * color] | m_pieces[bishop + max_pieces * color] | m_pieces[rook + max_pieces * color] | m_pieces[king + max_pieces * color] | m_pieces[queen + max_pieces * color] };
}

std::list<Board> Board::nextPossiblePositions(Color color)
{
	BitBoard whitePieces{ getPieces(white) };
	BitBoard blackPieces{ getPieces(black) };
	BitBoard allPieces{ whitePieces | blackPieces };
	std::list<Board> possiblePositions{};
	BitBoard myPawns{ m_pieces[pawn + max_pieces * color] };
	if (color == white)
	{
		while (myPawns)
		{
			BitBoard currentPawn = myPawns & (~myPawns + 1);
			if (!((currentPawn << 8) & allPieces))
			{
				Board newPosition{ *this };
				newPosition.m_pieces[pawn + max_pieces * white] = (newPosition.m_pieces[pawn + max_pieces * white] & (~currentPawn)) | currentPawn << 8;
				possiblePositions.push_back(newPosition);
			}

			if (currentPawn & constants::RANK_2 && !((currentPawn << 8) & allPieces) && !((currentPawn << 16) & allPieces))
			{
				Board newPosition{ *this };
				newPosition.m_pieces[pawn + max_pieces * white] = (newPosition.m_pieces[pawn + max_pieces * white] & (~currentPawn)) | currentPawn << 16;
				possiblePositions.push_back(newPosition);
			}

			if((currentPawn<<7&blackPieces) && (currentPawn & (~constants::FILE_A)))
			{
				Board newPosition{ *this };
				newPosition.m_pieces[pawn + max_pieces * white] = (newPosition.m_pieces[pawn + max_pieces * white] & (~currentPawn)) | currentPawn << 7;
				possiblePositions.push_back(newPosition);
			}

			if ((currentPawn<<9 & blackPieces) && (currentPawn & (~constants::FILE_H)))
			{
				Board newPosition{ *this };
				newPosition.m_pieces[pawn + max_pieces * white] = (newPosition.m_pieces[pawn + max_pieces * white] & (~currentPawn)) | currentPawn << 9;
				possiblePositions.push_back(newPosition);
			}

			myPawns &= myPawns - 1;
		}
	}
	return possiblePositions;

}