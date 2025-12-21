#include "PositionCalculator.h"
#include "utility.h"

bool PositionCalculator::positionSeen(const Board& board)
{
	return m_transposedPositions.contains(board);
}

void PositionCalculator::addPosition(const Board& board, std::list<Board>& positions)
{
	if (positionSeen(board) || !isKingSafe(board))
	{
		return;
	}
	positions.push_back(board);
	m_transposedPositions.insert(board);
}

std::list<Board> PositionCalculator::nextPossiblePositions(Board board)
{
	using BitBoard = std::uint64_t;
	BitBoard myPieces{ board.getPieces(board.m_currentTurn) };
	BitBoard enemyPieces{ board.getPieces(oppositeColor(board.m_currentTurn)) };
	BitBoard allPieces{ myPieces | enemyPieces };
	std::list<Board> possiblePositions{};
	BitBoard myPawns{ board.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] };

	while (myPawns)
	{
		BitBoard currentPawn = myPawns & (~myPawns + 1);
		if (!((shift(currentPawn, 8, board.m_currentTurn)) & allPieces))
		{

			if (shift(currentPawn, 8, board.m_currentTurn) & (constants::RANK_1 | constants::RANK_8))
			{
				for (std::size_t piece{ 1 }; piece < 5; ++piece)
				{
					Board newPosition{ board , 0 };
					newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] &= (~currentPawn);
					newPosition.m_pieces[piece + Board::max_pieces * board.m_currentTurn] |= shift(currentPawn, 8, board.m_currentTurn);
					addPosition(newPosition, possiblePositions);
				}
			}
			else {
				Board newPosition{ board , 0 };
				newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] & (~currentPawn)) | shift(currentPawn, 8, board.m_currentTurn);
				addPosition(newPosition, possiblePositions);
			}
		}

		if (((currentPawn & constants::RANK_2 && board.m_currentTurn == Board::white) || (currentPawn & constants::RANK_7 && board.m_currentTurn == Board::black)) && !(shift(currentPawn, 8, board.m_currentTurn) & allPieces) && !(shift(currentPawn, 16, board.m_currentTurn) & allPieces))
		{
			Board newPosition{ board , 0 };
			newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] & (~currentPawn)) | shift(currentPawn, 16, board.m_currentTurn);
			newPosition.m_enpassant = static_cast<std::uint8_t>(board.m_currentTurn == Board::white ? currentPawn >> 8 : currentPawn >> 48);
			addPosition(newPosition, possiblePositions);
		}

		if ((shift(currentPawn, 7, board.m_currentTurn) & enemyPieces) && (currentPawn & (~constants::FILE_A)))
		{
			if (shift(currentPawn, 7, board.m_currentTurn) & (constants::RANK_1 | constants::RANK_8))
			{
				for (std::size_t piece{ 1 }; piece < 5; ++piece)
				{
					Board newPosition{ board , 0 };
					newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] &= (~currentPawn);
					newPosition.m_pieces[piece + Board::max_pieces * board.m_currentTurn] |= shift(currentPawn, 7, board.m_currentTurn);
					addPosition(newPosition, possiblePositions);
				}
			}
			else {
				Board newPosition{ board , 0 };
				newPosition.eliminatePiece(shift(currentPawn, 7, board.m_currentTurn));
				newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] & (~currentPawn)) | shift(currentPawn, 7, board.m_currentTurn);
				addPosition(newPosition, possiblePositions);
			}
		}

		if ((shift(currentPawn, 9, board.m_currentTurn) & enemyPieces) && (currentPawn & (~constants::FILE_H)))
		{
			if (shift(currentPawn, 9, board.m_currentTurn) & (constants::RANK_1 | constants::RANK_8))
			{
				for (std::size_t piece{ 1 }; piece < 5; ++piece)
				{
					Board newPosition{ board , 0 };
					newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] &= (~currentPawn);
					newPosition.m_pieces[piece + Board::max_pieces * board.m_currentTurn] |= shift(currentPawn, 9, board.m_currentTurn);
					addPosition(newPosition, possiblePositions);
				}
			}
			else {
				Board newPosition{ board , 0 };
				newPosition.eliminatePiece(shift(currentPawn, 7, board.m_currentTurn));
				newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] & (~currentPawn)) | shift(currentPawn, 9, board.m_currentTurn);
				addPosition(newPosition, possiblePositions);
			}
		}

		if ((((currentPawn & constants::RANK_5) && board.m_currentTurn == Board::white) || ((currentPawn & constants::RANK_4) && board.m_currentTurn == Board::black)) && board.m_enpassant)
		{
			if ((board.m_enpassant & enpassantCheckRight(currentPawn, board.m_currentTurn)) && (currentPawn & (~constants::FILE_H)))
			{
				Board newPosition{  board , 0 };
				newPosition.eliminatePiece(currentPawn << 1);
				newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] & (~currentPawn)) | shift(currentPawn, board.m_currentTurn == Board::white ? 9 : 7, board.m_currentTurn);
				addPosition(newPosition, possiblePositions);
			}

			if ((board.m_enpassant & enpassantCheckLeft(currentPawn, board.m_currentTurn)) && (currentPawn & (~constants::FILE_A)))
			{
				Board newPosition{  board , 0 };
				newPosition.eliminatePiece(currentPawn >> 1);
				newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::pawn + Board::max_pieces * board.m_currentTurn] & (~currentPawn)) | shift(currentPawn, board.m_currentTurn == Board::white ? 7 : 9, board.m_currentTurn);
				addPosition(newPosition, possiblePositions);
			}
		}
		myPawns &= myPawns - 1;
	}

	BitBoard myKnights{ board.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] };

	while (myKnights)
	{
		BitBoard currentKnight = myKnights & (~myKnights + 1);
		if ((currentKnight & ~(constants::RANK_7 | constants::RANK_8) & ~constants::FILE_H) && !((currentKnight << 17) & myPieces))
		{
			Board newPosition{  board, 0 };
			newPosition.eliminatePiece(currentKnight << 17);
			newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] & (~currentKnight)) | currentKnight << 17;
			addPosition(newPosition, possiblePositions);
		}
		if ((currentKnight & ~(constants::RANK_7 | constants::RANK_8) & ~constants::FILE_A) && !((currentKnight << 15) & myPieces))
		{
			Board newPosition{  board, 0 };
			newPosition.eliminatePiece(currentKnight << 15);
			newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] & (~currentKnight)) | currentKnight << 15;
			addPosition(newPosition, possiblePositions);
		}
		if ((currentKnight & ~(constants::RANK_1 | constants::RANK_2) & ~constants::FILE_H) && !((currentKnight >> 15) & myPieces))
		{
			Board newPosition{  board, 0 };
			newPosition.eliminatePiece(currentKnight >> 15);
			newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] & (~currentKnight)) | currentKnight >> 15;
			addPosition(newPosition, possiblePositions);
		}
		if ((currentKnight & ~(constants::RANK_1 | constants::RANK_2) & ~constants::FILE_A) && !((currentKnight >> 17) & myPieces))
		{
			Board newPosition{  board, 0 };
			newPosition.eliminatePiece(currentKnight >> 17);
			newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] & (~currentKnight)) | currentKnight >> 17;
			addPosition(newPosition, possiblePositions);
		}
		if ((currentKnight & ~(constants::RANK_8) & ~(constants::FILE_G | constants::FILE_H)) && !((currentKnight << 10) & myPieces))
		{
			Board newPosition{  board, 0 };
			newPosition.eliminatePiece(currentKnight << 10);
			newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] & (~currentKnight)) | currentKnight << 10;
			addPosition(newPosition, possiblePositions);
		}
		if ((currentKnight & ~(constants::RANK_1) & ~(constants::FILE_G | constants::FILE_H)) && !((currentKnight >> 6) & myPieces))
		{
			Board newPosition{  board, 0 };
			newPosition.eliminatePiece(currentKnight >> 6);
			newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] & (~currentKnight)) | currentKnight >> 6;
			addPosition(newPosition, possiblePositions);
		}
		if ((currentKnight & ~(constants::RANK_1) & ~(constants::FILE_A | constants::FILE_B)) && !((currentKnight >> 10) & myPieces))
		{
			Board newPosition{  board, 0 };
			newPosition.eliminatePiece(currentKnight >> 10);
			newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] & (~currentKnight)) | currentKnight >> 10;
			addPosition(newPosition, possiblePositions);
		}
		if ((currentKnight & ~(constants::RANK_8) & ~(constants::FILE_A | constants::FILE_B)) && !((currentKnight << 6) & myPieces))
		{
			Board newPosition{  board, 0 };
			newPosition.eliminatePiece(currentKnight << 6);
			newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::knight + Board::max_pieces * board.m_currentTurn] & (~currentKnight)) | currentKnight << 6;
			addPosition(newPosition, possiblePositions);
		}

		myKnights &= myKnights - 1;
	}

	BitBoard myBishops{ board.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] };

	while (myBishops)
	{
		BitBoard currentBishop = myBishops & (~myBishops + 1);
		
		int jump{ 1 };
		while (true) // diagonal top right
		{
			BitBoard newBishop{ currentBishop << (9 * jump) };
			if (newBishop & myPieces || currentBishop & (constants::RANK_8 | constants::FILE_H))
			{
				break;
			}
			else if (newBishop & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newBishop);
				newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] & (~currentBishop)) | newBishop;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] & (~currentBishop)) | newBishop;
			addPosition(newPosition, possiblePositions);
			if (newBishop & (constants::RANK_8 | constants::FILE_H))
			{
				break;
			}
			++jump;
		}
		jump = 1 ;
		while (true) // diagonal top left
		{
			BitBoard newBishop{ currentBishop << (7 * jump) };
			if (newBishop & myPieces || currentBishop & (constants::RANK_8 | constants::FILE_A))
			{
				break;
			}
			else if (newBishop & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newBishop);
				newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] & (~currentBishop)) | newBishop;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] & (~currentBishop)) | newBishop;
			addPosition(newPosition, possiblePositions);
			if (newBishop & (constants::RANK_8 | constants::FILE_A))
			{
				break;
			}
			++jump;
		}
		jump = 1;
		while (true) // diagonal bottom right
		{
			BitBoard newBishop{ currentBishop >> (7 * jump) };
			if (newBishop & myPieces || currentBishop & (constants::RANK_1 | constants::FILE_H))
			{
				break;
			}
			else if (newBishop & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newBishop);
				newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] & (~currentBishop)) | newBishop;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] & (~currentBishop)) | newBishop;
			addPosition(newPosition, possiblePositions);
			if (newBishop & (constants::RANK_1 | constants::FILE_H))
			{
				break;
			}
			++jump;
		}
		jump = 1;
		while (true) // diagonal bottom right
		{
			BitBoard newBishop{ currentBishop >> (9 * jump) };
			if (newBishop & myPieces || currentBishop & (constants::RANK_1 | constants::FILE_A))
			{
				break;
			}
			else if (newBishop & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newBishop);
				newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] & (~currentBishop)) | newBishop;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::bishop + Board::max_pieces * board.m_currentTurn] & (~currentBishop)) | newBishop;
			addPosition(newPosition, possiblePositions);
			if (newBishop & (constants::RANK_1 | constants::FILE_A))
			{
				break;
			}
			++jump;
		}

		myBishops &= myBishops - 1;
	}

	BitBoard myRooks{ board.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] };

	while (myRooks)
	{
		BitBoard currentRook = myRooks & (~myRooks + 1);

		int jump{ 1 };
		while (true) // top
		{
			BitBoard newRook{ currentRook << (8 * jump) };
			if (newRook & myPieces || currentRook & constants::RANK_8)
			{
				break;
			}
			else if (newRook & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newRook);
				newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] & (~currentRook)) | newRook;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] & (~currentRook)) | newRook;
			addPosition(newPosition, possiblePositions);
			if (newRook & constants::RANK_8)
			{
				break;
			}
			++jump;
		}
		jump = 1;
		while (true) // bottom
		{
			BitBoard newRook{ currentRook >> (8 * jump) };
			if (newRook & myPieces || currentRook & constants::RANK_1)
			{
				break;
			}
			else if (newRook & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newRook);
				newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] & (~currentRook)) | newRook;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] & (~currentRook)) | newRook;
			addPosition(newPosition, possiblePositions);
			if (newRook & constants::RANK_1 )
			{
				break;
			}
			++jump;
		}
		jump = 1;
		while (true) // right
		{
			BitBoard newRook{ currentRook << jump };
			if (newRook & myPieces || currentRook & constants::FILE_H)
			{
				break;
			}
			else if (newRook & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newRook);
				newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] & (~currentRook)) | newRook;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] & (~currentRook)) | newRook;
			addPosition(newPosition, possiblePositions);
			if (newRook & constants::FILE_H)
			{
				break;
			}
			++jump;
		}
		jump = 1;
		while (true) // left
		{
			BitBoard newRook{ currentRook >> jump };
			if (newRook & myPieces || currentRook & constants::FILE_A)
			{
				break;
			}
			else if (newRook & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newRook);
				newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] & (~currentRook)) | newRook;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::rook + Board::max_pieces * board.m_currentTurn] & (~currentRook)) | newRook;
			addPosition(newPosition, possiblePositions);
			if (newRook & constants::FILE_A)
			{
				break;
			}
			++jump;
		}

		myRooks &= myRooks - 1;
	}

	BitBoard myQueens{ board.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] };

	while (myQueens)
	{
		BitBoard currentQueen = myQueens & (~myQueens + 1);

		int jump{ 1 };
		while (true) // top
		{
			BitBoard newQueen{ currentQueen << (8 * jump) };
			if (newQueen & myPieces || currentQueen & constants::RANK_8)
			{
				break;
			}
			else if (newQueen & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newQueen);
				newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
			addPosition(newPosition, possiblePositions);
			if (newQueen & constants::RANK_8)
			{
				break;
			}
			++jump;
		}
		jump = 1;
		while (true) // bottom
		{
			BitBoard newQueen{ currentQueen >> (8 * jump) };
			if (newQueen & myPieces || currentQueen & constants::RANK_1)
			{
				break;
			}
			else if (newQueen & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newQueen);
				newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
			addPosition(newPosition, possiblePositions);
			if (newQueen & constants::RANK_1)
			{
				break;
			}
			++jump;
		}
		jump = 1;
		while (true) // right
		{
			BitBoard newQueen{ currentQueen << jump };
			if (newQueen & myPieces || currentQueen & constants::FILE_H)
			{
				break;
			}
			else if (newQueen & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newQueen);
				newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
			addPosition(newPosition, possiblePositions);
			if (newQueen & constants::FILE_H)
			{
				break;
			}
			++jump;
		}
		jump = 1;
		while (true) // left
		{
			BitBoard newQueen{ currentQueen >> jump };
			if (newQueen & myPieces || currentQueen & constants::FILE_A)
			{
				break;
			}
			else if (newQueen & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newQueen);
				newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
			addPosition(newPosition, possiblePositions);
			if (newQueen & constants::FILE_A)
			{
				break;
			}
			++jump;
		}

		jump = 1;
		while (true) // diagonal top right
		{
			BitBoard newQueen{ currentQueen << (9 * jump) };
			if (newQueen & myPieces || currentQueen & (constants::RANK_8 | constants::FILE_H))
			{
				break;
			}
			else if (newQueen & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newQueen);
				newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
			addPosition(newPosition, possiblePositions);
			if (newQueen & (constants::RANK_8 | constants::FILE_H))
			{
				break;
			}
			++jump;
		}
		jump = 1;
		while (true) // diagonal top left
		{
			BitBoard newQueen{ currentQueen << (7 * jump) };
			if (newQueen & myPieces || currentQueen & (constants::RANK_8 | constants::FILE_A))
			{
				break;
			}
			else if (newQueen & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newQueen);
				newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
			addPosition(newPosition, possiblePositions);
			if (newQueen & (constants::RANK_8 | constants::FILE_A))
			{
				break;
			}
			++jump;
		}
		jump = 1;
		while (true) // diagonal bottom right
		{
			BitBoard newQueen{ currentQueen >> (7 * jump) };
			if (newQueen & myPieces || currentQueen & (constants::RANK_1 | constants::FILE_H))
			{
				break;
			}
			else if (newQueen & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newQueen);
				newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
			addPosition(newPosition, possiblePositions);
			if (newQueen & (constants::RANK_1 | constants::FILE_H))
			{
				break;
			}
			++jump;
		}
		jump = 1;
		while (true) // diagonal bottom right
		{
			BitBoard newQueen{ currentQueen >> (9 * jump) };
			if (newQueen & myPieces || currentQueen & (constants::RANK_1 | constants::FILE_A))
			{
				break;
			}
			else if (newQueen & enemyPieces)
			{
				Board newPosition{ board, 0 };
				newPosition.eliminatePiece(newQueen);
				newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
				addPosition(newPosition, possiblePositions);
				break;
			}
			Board newPosition{ board, 0 };
			newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] = (newPosition.m_pieces[Board::queen + Board::max_pieces * board.m_currentTurn] & (~currentQueen)) | newQueen;
			addPosition(newPosition, possiblePositions);
			if (newQueen & (constants::RANK_1 | constants::FILE_A))
			{
				break;
			}
			++jump;
		}

		myQueens &= myQueens - 1;
	}

	BitBoard myKing{ board.m_pieces[Board::king + Board::max_pieces * board.m_currentTurn] };

	//top check
	BitBoard newKing{ myKing << 8 };
	if (myKing & ~constants::RANK_8 && newKing & ~myPieces)
	{
		Board newPosition{ board, 0 };
		newPosition.eliminatePiece(newKing);
		newPosition.m_pieces[Board::king + Board::max_pieces * board.m_currentTurn] = newKing;
		addPosition(newPosition, possiblePositions);

	}
	 //bottom check
	newKing = myKing >> 8;
	if (myKing & ~constants::RANK_1 && newKing & ~myPieces)
	{
		Board newPosition{ board, 0 };
		newPosition.eliminatePiece(newKing);
		newPosition.m_pieces[Board::king + Board::max_pieces * board.m_currentTurn] = newKing;
		addPosition(newPosition, possiblePositions);

	}
	//left check
	newKing = myKing >> 1;
	if (myKing & ~constants::FILE_A && newKing & ~myPieces)
	{
		Board newPosition{ board, 0 };
		newPosition.eliminatePiece(newKing);
		newPosition.m_pieces[Board::king + Board::max_pieces * board.m_currentTurn] = newKing;
		addPosition(newPosition, possiblePositions);

	}
	//right check
	newKing = myKing << 1;
	if (myKing & ~constants::FILE_H && newKing & ~myPieces)
	{
		Board newPosition{ board, 0 };
		newPosition.eliminatePiece(newKing);
		newPosition.m_pieces[Board::king + Board::max_pieces * board.m_currentTurn] = newKing;
		addPosition(newPosition, possiblePositions);

	}

	//top right check
	newKing= myKing << 9;
	if (myKing & ~constants::RANK_8 & ~constants::FILE_H && newKing & ~myPieces)
	{
		Board newPosition{ board, 0 };
		newPosition.eliminatePiece(newKing);
		newPosition.m_pieces[Board::king + Board::max_pieces * board.m_currentTurn] = newKing;
		addPosition(newPosition, possiblePositions);
	}
	//bottom left check
	newKing = myKing >> 9;
	if (myKing & ~constants::RANK_1 & ~constants::FILE_A && newKing & ~myPieces)
	{
		Board newPosition{ board, 0 };
		newPosition.eliminatePiece(newKing);
		newPosition.m_pieces[Board::king + Board::max_pieces * board.m_currentTurn] = newKing;
		addPosition(newPosition, possiblePositions);

	}
	//top left check
	newKing = myKing << 7;
	if (myKing & ~constants::FILE_A & ~constants::RANK_8 && newKing & ~myPieces)
	{
		Board newPosition{ board, 0 };
		newPosition.eliminatePiece(newKing);
		newPosition.m_pieces[Board::king + Board::max_pieces * board.m_currentTurn] = newKing;
		addPosition(newPosition, possiblePositions);
	}
	//bottom right check
	newKing = myKing >> 7;
	if (myKing & ~constants::FILE_H & ~constants::RANK_1 && newKing & ~myPieces)
	{
		Board newPosition{ board, 0 };
		newPosition.eliminatePiece(newKing);
		newPosition.m_pieces[Board::king + Board::max_pieces * board.m_currentTurn] = newKing;
		addPosition(newPosition, possiblePositions);
	}


	return possiblePositions;
}

bool PositionCalculator::isKingSafe(const Board& board)
{
	using BitBoard = std::uint64_t;
	BitBoard myKing{ board.m_pieces[Board::king + oppositeColor(board.m_currentTurn) * Board::max_pieces] };
	BitBoard myPieces{ board.getPieces(oppositeColor(board.m_currentTurn)) };
	BitBoard enemyPieces{ board.getPieces(board.m_currentTurn) };
	BitBoard enemyRooks{ board.m_pieces[Board::rook + board.m_currentTurn * Board::max_pieces] };
	BitBoard enemyQueens{ board.m_pieces[Board::queen + board.m_currentTurn * Board::max_pieces] };
	BitBoard enemyBishops{  board.m_pieces[Board::bishop + board.m_currentTurn * Board::max_pieces] }; 
	BitBoard enemyKnights{ board.m_pieces[Board::knight + board.m_currentTurn * Board::max_pieces] };
	BitBoard enemyPawns{ board.m_pieces[Board::pawn + board.m_currentTurn * Board::max_pieces] };
	BitBoard enemyKing{ board.m_pieces[Board::king + board.m_currentTurn * Board::max_pieces] };

	//top check
	BitBoard squareCheck{ myKing << 8 };
	if (myKing & (~constants::RANK_8))
	{
		while (true)
		{
			if (squareCheck & myPieces)
			{
				break;
			}
			if (squareCheck & enemyPieces)
			{
				if (squareCheck & (enemyKing | enemyQueens | enemyRooks))
				{
					return false;
				}
				else
				{
					break;
				}
			}
			if (squareCheck & constants::RANK_8)
			{
				break;
			}
			squareCheck <<= 8;
		}
	}

	//Bottom check
	squareCheck = myKing >> 8;
	if (myKing & (~constants::RANK_1))
	{
		while (true)
		{
			if (squareCheck & myPieces)
			{
				break;
			}
			if (squareCheck & enemyPieces)
			{
				if (squareCheck & (enemyKing | enemyQueens | enemyRooks))
				{
					return false;
				}
				else
				{
					break;
				}
			}
			if (squareCheck & constants::RANK_1)
			{
				break;
			}
			squareCheck >>= 8;
		}
	}

	//Right check
	squareCheck = myKing << 1;
	if (myKing & (~constants::FILE_H))
	{
		while (true)
		{
			if (squareCheck & myPieces)
			{
				break;
			}
			if (squareCheck & enemyPieces)
			{
				if (squareCheck & (enemyKing | enemyQueens | enemyRooks))
				{
					return false;
				}
				else
				{
					break;
				}
			}
			if (squareCheck & constants::FILE_H)
			{
				break;
			}
			squareCheck <<= 1;
		}
	}

	//Left check
	squareCheck = myKing >> 1;
	if (myKing & (~constants::FILE_A))
	{
		while (true)
		{
			if (squareCheck & myPieces)
			{
				break;
			}
			if (squareCheck & enemyPieces)
			{
				if (squareCheck & (enemyKing | enemyQueens | enemyRooks))
				{
					return false;
				}
				else
				{
					break;
				}
			}
			if (squareCheck & constants::FILE_A)
			{
				break;
			}
			squareCheck >>= 1;
		}
	}

	//top right check
	squareCheck = myKing << 9;
	bool firstSquare{ true };
	if (myKing & ~constants::RANK_8 & ~constants::FILE_H)
	{
		while (true)
		{
			if (squareCheck & myPieces)
			{
				break;
			}
			if (squareCheck & enemyPieces)
			{
				if (squareCheck & (enemyKing | enemyQueens | enemyBishops))
				{
					return false;
				}
				else if (firstSquare && (squareCheck & enemyPawns) && (board.m_currentTurn == Board::black))
				{
					return false;
				}
				else
				{
					break;
				}
			}
			if (squareCheck & (constants::RANK_8 | constants::FILE_H))
			{
				break;
			}
			firstSquare = false;
			squareCheck <<= 9;
		}
	}

	//top left check
	squareCheck = myKing << 7;
	firstSquare = true;
	if (myKing & ~constants::RANK_8 & ~constants::FILE_A)
	{
		while (true)
		{
			if (squareCheck & myPieces)
			{
				break;
			}
			if (squareCheck & enemyPieces)
			{
				if (squareCheck & (enemyKing | enemyQueens | enemyBishops))
				{
					return false;
				}
				else if (firstSquare && (squareCheck & enemyPawns) && (board.m_currentTurn == Board::black))
				{
					return false;
				}
				else
				{
					break;
				}
			}
			if (squareCheck & (constants::RANK_8 | constants::FILE_A))
			{
				break;
			}
			firstSquare = false;
			squareCheck <<= 7;
		}
	}

	//bottom right check
	squareCheck = myKing >> 7;
	firstSquare = true;
	if (myKing & ~constants::RANK_1 & ~constants::FILE_H)
	{
		while (true)
		{
			if (squareCheck & myPieces)
			{
				break;
			}
			if (squareCheck & enemyPieces)
			{
				if (squareCheck & (enemyKing | enemyQueens | enemyBishops))
				{
					return false;
				}
				else if (firstSquare && (squareCheck & enemyPawns) && (board.m_currentTurn == Board::white))
				{
					return false;
				}
				else
				{
					break;
				}
			}
			if (squareCheck & (constants::RANK_1 | constants::FILE_H))
			{
				break;
			}
			firstSquare = false;
			squareCheck >>= 7;
		}
	}

	//bottom left check
	squareCheck = myKing >> 9;
	firstSquare = true;
	if (myKing & ~constants::RANK_1 & ~constants::FILE_A)
	{
		while (true)
		{
			if (squareCheck & myPieces)
			{
				break;
			}
			if (squareCheck & enemyPieces)
			{
				if (squareCheck & (enemyKing | enemyQueens | enemyBishops))
				{
					return false;
				}
				else if (firstSquare && (squareCheck & enemyPawns) && (board.m_currentTurn == Board::white))
				{
					return false;
				}
				else
				{
					break;
				}
			}
			if (squareCheck & (constants::RANK_1 | constants::FILE_A))
			{
				break;
			}
			firstSquare = false;
			squareCheck >>= 9;
		}
	}

	//knight checks
	if ((myKing & ~(constants::RANK_7 | constants::RANK_8) & ~constants::FILE_H) && ((myKing<< 17) & enemyKnights))
	{
		return false;
	}
	if ((myKing & ~(constants::RANK_7 | constants::RANK_8) & ~constants::FILE_A) && ((myKing << 15) & enemyKnights))
	{
		return false;
	}
	if ((myKing & ~(constants::RANK_1 | constants::RANK_2) & ~constants::FILE_H) && ((myKing >> 15) & enemyKnights))
	{
		return false;
	}
	if ((myKing & ~(constants::RANK_1 | constants::RANK_2) & ~constants::FILE_A) && ((myKing >> 17) & enemyKnights))
	{
		return false;
	}
	if ((myKing & ~(constants::RANK_8) & ~(constants::FILE_G | constants::FILE_H)) && ((myKing << 10) & enemyKnights))
	{
		return false;
	}
	if ((myKing & ~(constants::RANK_1) & ~(constants::FILE_G | constants::FILE_H)) && ((myKing >> 6) & enemyKnights))
	{
		return false;
	}
	if ((myKing & ~(constants::RANK_1) & ~(constants::FILE_A | constants::FILE_B)) && ((myKing >> 10) & enemyKnights))
	{
		return false;
	}
	if ((myKing & ~(constants::RANK_8) & ~(constants::FILE_A | constants::FILE_B)) && ((myKing << 6) & enemyKnights))
	{
		return false;
	}

	return true;
}