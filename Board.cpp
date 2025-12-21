#include "Board.h"
#include "utility.h"

using BitBoard = std::uint64_t;

Board::Board()
{
	m_currentTurn = white;

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

std::list<Board> Board::nextPossiblePositions()
{
	BitBoard myPieces{ getPieces(m_currentTurn)};
	BitBoard enemyPieces{ getPieces(oppositeColor(m_currentTurn)) };
	BitBoard allPieces{ myPieces | enemyPieces};
	std::list<Board> possiblePositions{};
	BitBoard myPawns{ m_pieces[pawn + max_pieces * m_currentTurn] };
	
	while (myPawns)
	{
		BitBoard currentPawn = myPawns & (~myPawns + 1);
		if (!((shift(currentPawn,8,m_currentTurn)) & allPieces))
		{
			
			if(shift(currentPawn, 8, m_currentTurn)&(constants::RANK_1|constants::RANK_8))
			{ 
				for (std::size_t piece{ 1 }; piece < 5; ++piece)
				{
					Board newPosition{ *this , 0 };
					newPosition.m_pieces[pawn + max_pieces * m_currentTurn] &= (~currentPawn);
					newPosition.m_pieces[piece + max_pieces * m_currentTurn] |= shift(currentPawn, 8, m_currentTurn);
					possiblePositions.push_back(newPosition);
				}
			}
			else {
				Board newPosition{ *this , 0 };
				newPosition.m_pieces[pawn + max_pieces * m_currentTurn] = (newPosition.m_pieces[pawn + max_pieces * m_currentTurn] & (~currentPawn)) | shift(currentPawn, 8, m_currentTurn);
				possiblePositions.push_back(newPosition);
			}
		}

		if (((currentPawn & constants::RANK_2 && m_currentTurn == white) || (currentPawn & constants::RANK_7 && m_currentTurn == black))&& !(shift(currentPawn, 8, m_currentTurn) & allPieces) && !(shift(currentPawn, 16, m_currentTurn) & allPieces))
		{
			Board newPosition{ *this , 0 };
			newPosition.m_pieces[pawn + max_pieces * m_currentTurn] = (newPosition.m_pieces[pawn + max_pieces * m_currentTurn] & (~currentPawn)) | shift(currentPawn, 16, m_currentTurn);
			newPosition.m_enpassant = static_cast<std::uint8_t>(m_currentTurn==white ? currentPawn >> 8 : currentPawn >> 48);
			possiblePositions.push_back(newPosition);
		}

		if ((shift(currentPawn, 7, m_currentTurn) & enemyPieces) && (currentPawn & (~constants::FILE_A)))
		{
			if (shift(currentPawn, 7, m_currentTurn) & (constants::RANK_1 | constants::RANK_8))
			{
				for (std::size_t piece{ 1 }; piece < 5; ++piece)
				{
					Board newPosition{ *this , 0 };
					newPosition.m_pieces[pawn + max_pieces * m_currentTurn] &= (~currentPawn);
					newPosition.m_pieces[piece + max_pieces * m_currentTurn] |= shift(currentPawn, 7, m_currentTurn);
					possiblePositions.push_back(newPosition);
				}
			}
			else{
				Board newPosition{ *this , 0 };
				newPosition.eliminatePiece(shift(currentPawn, 7, m_currentTurn));
				newPosition.m_pieces[pawn + max_pieces * m_currentTurn] = (newPosition.m_pieces[pawn + max_pieces * m_currentTurn] & (~currentPawn)) | shift(currentPawn, 7, m_currentTurn);
				possiblePositions.push_back(newPosition);
			}
		}

		if ((shift(currentPawn, 9, m_currentTurn) & enemyPieces) && (currentPawn & (~constants::FILE_H)))
		{
			if (shift(currentPawn, 9, m_currentTurn) & (constants::RANK_1 | constants::RANK_8))
			{
				for (std::size_t piece{ 1 }; piece < 5; ++piece)
				{
					Board newPosition{ *this , 0 };
					newPosition.m_pieces[pawn + max_pieces * m_currentTurn] &= (~currentPawn);
					newPosition.m_pieces[piece + max_pieces * m_currentTurn] |= shift(currentPawn, 9, m_currentTurn);
					possiblePositions.push_back(newPosition);
				}
			}
			else {
				Board newPosition{ *this , 0 };
				newPosition.eliminatePiece(shift(currentPawn, 7, m_currentTurn));
				newPosition.m_pieces[pawn + max_pieces * m_currentTurn] = (newPosition.m_pieces[pawn + max_pieces * m_currentTurn] & (~currentPawn)) | shift(currentPawn, 9, m_currentTurn);
				possiblePositions.push_back(newPosition);
			}
		}

		if ((((currentPawn & constants::RANK_5) && m_currentTurn == white) || ((currentPawn & constants::RANK_4) && m_currentTurn == black) )&& m_enpassant)
		{
			if ((m_enpassant & enpassantCheckRight(currentPawn,m_currentTurn)) && (currentPawn & (~constants::FILE_H)))
			{
				Board newPosition{ *this , 0 };
				newPosition.eliminatePiece(currentPawn << 1);
				newPosition.m_pieces[pawn + max_pieces * m_currentTurn] = (newPosition.m_pieces[pawn + max_pieces * m_currentTurn] & (~currentPawn)) | shift(currentPawn, m_currentTurn == white ? 9 : 7, m_currentTurn);
				possiblePositions.push_back(newPosition);
			}

			if ((m_enpassant & enpassantCheckLeft(currentPawn, m_currentTurn)) && (currentPawn & (~constants::FILE_A)))
			{
				Board newPosition{ *this , 0 };
				newPosition.eliminatePiece(currentPawn >> 1);
				newPosition.m_pieces[pawn + max_pieces * m_currentTurn] = (newPosition.m_pieces[pawn + max_pieces * m_currentTurn] & (~currentPawn)) | shift(currentPawn, m_currentTurn == white ? 7 : 9, m_currentTurn);
				possiblePositions.push_back(newPosition);
			}
		}
		myPawns &= myPawns - 1;
	}

	BitBoard myKnights{ m_pieces[knight + max_pieces * m_currentTurn] };

	while (myKnights)
	{
		BitBoard currentKnight = myKnights & (~myKnights + 1);
		if ((currentKnight & ~(constants::RANK_7 | constants::RANK_8) & ~constants::FILE_H) && !((currentKnight << 17) & myPieces))
		{
			Board newPosition{ *this, 0 };
			newPosition.eliminatePiece(currentKnight << 17);
			newPosition.m_pieces[knight + max_pieces * m_currentTurn] = (newPosition.m_pieces[knight + max_pieces * m_currentTurn] & (~currentKnight)) | currentKnight << 17;
			possiblePositions.push_back(newPosition);
		}
		if ((currentKnight & ~(constants::RANK_7 | constants::RANK_8) & ~constants::FILE_A) && !((currentKnight << 15) & myPieces))
		{
			Board newPosition{ *this, 0 };
			newPosition.eliminatePiece(currentKnight << 15);
			newPosition.m_pieces[knight + max_pieces * m_currentTurn] = (newPosition.m_pieces[knight + max_pieces * m_currentTurn] & (~currentKnight)) | currentKnight << 15;
			possiblePositions.push_back(newPosition);
		}
		if ((currentKnight & ~(constants::RANK_1 | constants::RANK_2) & ~constants::FILE_H) && !((currentKnight >> 15) & myPieces))
		{
			Board newPosition{ *this, 0 };
			newPosition.eliminatePiece(currentKnight >> 15);
			newPosition.m_pieces[knight + max_pieces * m_currentTurn] = (newPosition.m_pieces[knight + max_pieces * m_currentTurn] & (~currentKnight)) | currentKnight >> 15;
			possiblePositions.push_back(newPosition);
		}
		if ((currentKnight & ~(constants::RANK_1 | constants::RANK_2) & ~constants::FILE_A) && !((currentKnight >> 17) & myPieces))
		{
			Board newPosition{ *this, 0 };
			newPosition.eliminatePiece(currentKnight >> 17);
			newPosition.m_pieces[knight + max_pieces * m_currentTurn] = (newPosition.m_pieces[knight + max_pieces * m_currentTurn] & (~currentKnight)) | currentKnight >> 17;
			possiblePositions.push_back(newPosition);
		}
		if ((currentKnight & ~(constants::RANK_8) & ~(constants::FILE_G|constants::FILE_H)) &&!((currentKnight << 10) & myPieces))
		{
			Board newPosition{ *this, 0 };
			newPosition.eliminatePiece(currentKnight << 10);
			newPosition.m_pieces[knight + max_pieces * m_currentTurn] = (newPosition.m_pieces[knight + max_pieces * m_currentTurn] & (~currentKnight)) | currentKnight << 10;
			possiblePositions.push_back(newPosition);
		}
		if ((currentKnight & ~(constants::RANK_1) & ~(constants::FILE_G | constants::FILE_H)) && !((currentKnight >> 6) & myPieces))
		{
			Board newPosition{ *this, 0 };
			newPosition.eliminatePiece(currentKnight >> 6);
			newPosition.m_pieces[knight + max_pieces * m_currentTurn] = (newPosition.m_pieces[knight + max_pieces * m_currentTurn] & (~currentKnight)) | currentKnight >> 6;
			possiblePositions.push_back(newPosition);
		}
		if ((currentKnight & ~(constants::RANK_1) & ~(constants::FILE_A | constants::FILE_B)) && !((currentKnight >> 10) & myPieces))
		{
			Board newPosition{ *this, 0 };
			newPosition.eliminatePiece(currentKnight >> 10);
			newPosition.m_pieces[knight + max_pieces * m_currentTurn] = (newPosition.m_pieces[knight + max_pieces * m_currentTurn] & (~currentKnight)) | currentKnight >> 10;
			possiblePositions.push_back(newPosition);
		}
		if ((currentKnight & ~(constants::RANK_8) & ~(constants::FILE_A | constants::FILE_B)) && !((currentKnight << 6) & myPieces))
		{
			Board newPosition{ *this, 0 };
			newPosition.eliminatePiece(currentKnight << 6);
			newPosition.m_pieces[knight + max_pieces * m_currentTurn] = (newPosition.m_pieces[knight + max_pieces * m_currentTurn] & (~currentKnight)) | currentKnight << 6;
			possiblePositions.push_back(newPosition);
		}

		myKnights &= myKnights - 1;
	}

	return possiblePositions;

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