#pragma once
#include <cstdint>
#include <array>
#include "settings.h"
#include <list>
#include <iostream>
#include <bitset>

class Board
{
public:
	using BitBoard = std::uint64_t;
	std::array<BitBoard, 12> m_pieces{};
	std::uint8_t m_enpassant{};
	static constexpr std::array<char, 12> indexToChar{ 'P','N','B','R','Q','K','p','n','b','r','q','k' };

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
	std::list<Board> nextPossiblePositions();
	Board();
	Board(const Board& board) = default;
	Board(const Board& board, std::uint8_t enpassant)
		: m_pieces{board.m_pieces}, m_enpassant{enpassant}
	{
		m_currentTurn = (board.m_currentTurn == white ? black : white);
	}
	std::array<BitBoard, 12> getBitBoards() { return m_pieces; }
	void display();
	void eliminatePiece(BitBoard position);
	Color getCurrentTurn() { return m_currentTurn; }

private:
	Color m_currentTurn{};
};

