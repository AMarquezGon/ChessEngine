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
	std::uint8_t m_castling{};
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
	
	Board();
	Board(const Board& board) = default;
	Board(const Board& board, std::uint8_t enpassant)
		: m_pieces{board.m_pieces}, m_enpassant{enpassant}
	{
		m_currentTurn = (board.m_currentTurn == white ? black : white);
	}
	std::array<BitBoard, 12> getBitBoards() const { return m_pieces; }
	void display() const;
	void eliminatePiece(BitBoard position);
	Color getCurrentTurn() { return m_currentTurn; }
	void setBit(Piece piece, Color color, int file, int rank);
	bool getBit(Piece piece, Color color, int file, int rank);
	void clearBit(Piece piece, Color color, int file, int rank);
	BitBoard getPieces(Color color) const;
	bool operator==(const Board& other) const;

public:
	Color m_currentTurn{};
};

