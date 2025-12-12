#pragma once
#include <cstdint>
#include <array>
#include "settings.h"
#include <list>

class Board
{
public:
	using BitBoard = std::uint64_t;
	std::array<BitBoard, 12> m_pieces{};

public:
	enum File
	{
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H
	};

	enum Color
	{
		white,
		black,
	};

	enum Piece
	{
		pawn,
		knight,
		bishop,
		rook,
		queen,
		king,
		max_pieces
	};
	void setBit(Piece piece, Color color, int file, int rank);
	bool getBit(Piece piece, Color color, int file, int rank);
	void clearBit(Piece piece, Color color, int file, int rank);
	BitBoard getPieces(Color color);
	std::list<Board> nextPossiblePositions(Color color);
	Board();

};

