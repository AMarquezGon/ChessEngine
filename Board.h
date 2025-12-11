#pragma once
#include <cstdint>
#include <array>

class Board
{
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

private:
	using BitBoard = std::uint64_t;
	void setBit(Piece piece, Color color, int file, int rank);
	BitBoard getBit(Piece piece, Color color, int file, int rank);
	void clearBit(Piece piece, Color color, int file, int rank);
	std::array<BitBoard, 12> m_pieces{};
};

