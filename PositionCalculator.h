#pragma once
#include <unordered_set>
#include "Board.h"


struct KeyHash {
    std::size_t operator()(const Board& k) const noexcept {
        std::size_t h = 0;

        auto combine = [&](std::size_t v) {
            h ^= v + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
            };

        for (uint64_t v : k.m_pieces)
            combine(std::hash<uint64_t>{}(v));

        combine(std::hash<uint8_t>{}(k.m_enpassant));
        combine(std::hash<uint8_t>{}(k.m_castling));
        combine(std::hash<bool>{}(k.m_currentTurn));

        return h;
    }
};

class PositionCalculator
{
private:
    
    std::unordered_set<Board, KeyHash> m_transposedPositions{};

public:
    std::list<Board> nextPossiblePositions(Board board);
    void addPosition(const Board& board, std::list<Board>& positions);
    bool positionSeen(const Board& board);
    bool kingSafe(const Board& board);
};

