#pragma once
#include "Board.h"
#include "settings.h"

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

constexpr std::uint64_t shift(std::uint64_t value, std::uint8_t shift, Board::Color color)
{
    return color == Board::white ? value << shift : value >> shift;
}

constexpr std::uint64_t enpassantCheckRight(std::uint64_t value, Board::Color color)
{
    return color == Board::white ? value >> 31 : value >> 23;
}

constexpr std::uint64_t enpassantCheckLeft(std::uint64_t value, Board::Color color)
{
    return color == Board::white ? value >> 33 : value >> 25;
}

bool isKingSafe(const Board& board)
{
	using BitBoard = std::uint64_t;
	BitBoard myKing{ board.m_pieces[Board::king + oppositeColor(board.m_currentTurn) * Board::max_pieces] };
	BitBoard myPieces{ board.getPieces(oppositeColor(board.m_currentTurn)) };
	BitBoard enemyPieces{ board.getPieces(board.m_currentTurn) };
	BitBoard enemyRooks{ board.m_pieces[Board::rook + board.m_currentTurn * Board::max_pieces] };
	BitBoard enemyQueens{ board.m_pieces[Board::queen + board.m_currentTurn * Board::max_pieces] };
	BitBoard enemyBishops{ board.m_pieces[Board::bishop + board.m_currentTurn * Board::max_pieces] };
	BitBoard enemyKnights{ board.m_pieces[Board::knight + board.m_currentTurn * Board::max_pieces] };
	BitBoard enemyPawns{ board.m_pieces[Board::pawn + board.m_currentTurn * Board::max_pieces] };
	BitBoard enemyKing{ board.m_pieces[Board::king + board.m_currentTurn * Board::max_pieces] };

	//top check
	BitBoard squareCheck{ myKing << 8 };
	bool firstSquare{ true };
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
				if (squareCheck & ((enemyKing & firstSquare) | enemyQueens | enemyRooks))
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
			firstSquare = false;
			squareCheck <<= 8;
		}
	}

	//Bottom check
	squareCheck = myKing >> 8;
	firstSquare = true;
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
				if (squareCheck & ((enemyKing & firstSquare) | enemyQueens | enemyRooks))
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
			firstSquare = false;
			squareCheck >>= 8;
		}
	}

	//Right check
	squareCheck = myKing << 1;
	firstSquare = true;
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
				if (squareCheck & ((enemyKing & firstSquare) | enemyQueens | enemyRooks))
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
			firstSquare = false;
			squareCheck <<= 1;
		}
	}

	//Left check
	squareCheck = myKing >> 1;
	firstSquare = true;
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
				if (squareCheck & ((enemyKing & firstSquare) | enemyQueens | enemyRooks))
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
			firstSquare = false;
			squareCheck >>= 1;
		}
	}

	//top right check
	squareCheck = myKing << 9;
	firstSquare = true;
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
				if (squareCheck & ((enemyKing & firstSquare) | enemyQueens | enemyBishops))
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
				if (squareCheck & ((enemyKing & firstSquare) | enemyQueens | enemyBishops))
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
				if (squareCheck & ((enemyKing & firstSquare) | enemyQueens | enemyBishops))
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
				if (squareCheck & ((enemyKing & firstSquare) | enemyQueens | enemyBishops))
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
	if ((myKing & ~(constants::RANK_7 | constants::RANK_8) & ~constants::FILE_H) && ((myKing << 17) & enemyKnights))
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

bool castleQueensideCheck(Board& b)
{
    if (!(b.m_castling & (1 << (b.m_currentTurn * 2 + 1)))) //if no castling is possible due to king or rook movement
        return false;

    std::uint64_t myPieces{ b.getPieces(b.m_currentTurn) };
    std::uint64_t enemyPieces{ b.getPieces(oppositeColor(b.m_currentTurn)) };
    std::uint64_t allPieces{ myPieces | enemyPieces };

    if ((b.m_pieces[Board::king + Board::max_pieces * b.m_currentTurn] >> 1 | b.m_pieces[Board::king + Board::max_pieces * b.m_currentTurn] >> 2 | b.m_pieces[Board::king + Board::max_pieces * b.m_currentTurn] >> 2) & allPieces) // if there is a piece between the rook and the king
        return false;

    Board positionCFile{ b,0 };
    Board positionDFile{ b,0 };

    positionCFile.m_pieces[Board::king + Board::max_pieces * b.m_currentTurn] >>= 2;
    positionDFile.m_pieces[Board::king + Board::max_pieces * b.m_currentTurn] >>= 1;

    if ((!isKingSafe(positionCFile)) || (!isKingSafe(positionDFile)))
        return false;
    return true;
}

bool castleKingsideCheck(Board& b)
{
    if (!(b.m_castling & (1<<(b.m_currentTurn*2)))) //if no castling is possible due to king or rook movement
        return false;
    
    std::uint64_t myPieces{ b.getPieces(b.m_currentTurn) };
    std::uint64_t enemyPieces{ b.getPieces(oppositeColor(b.m_currentTurn)) };
    std::uint64_t allPieces{ myPieces | enemyPieces };

    if ((b.m_pieces[Board::king + Board::max_pieces * b.m_currentTurn] << 1 | b.m_pieces[Board::king + Board::max_pieces * b.m_currentTurn] << 2) & allPieces) // if there is a piece between the rook and the king
        return false;

    Board positionFFile{ b,0 };
    Board positionGFile{ b,0 };

    positionFFile.m_pieces[Board::king + Board::max_pieces * b.m_currentTurn] <<= 1;
    positionGFile.m_pieces[Board::king + Board::max_pieces * b.m_currentTurn] <<= 2;

    if ((!isKingSafe(positionFFile)) || (!isKingSafe(positionGFile)))
        return false;
    return true;
}
//Piece moves
//Bit functions (move out of class)
//Board representation
