#include <iostream>
#include "Board.h"

int main()
{
    Board b{};
    b.m_pieces[Board::queen + Board::max_pieces] = 1ULL << 19;
    std::list<Board> possiblePositions{b.nextPossiblePositions(Board::white)};
    int it{ 1 };
    std::cout << "Enemy queens" << '\n';
    for (int rank{ 8 }; rank > 0; --rank)
    {
        for (int file{ 1 }; file < 9; ++file)
        {
            std::cout << b.getBit(Board::queen, Board::black, file, rank) << ' ';
        }
        std::cout << '\n';
    }

    for (Board board : possiblePositions)
    {
        std::cout << "Board number " << it << '\n';
        for (int rank{ 8 }; rank > 0; --rank)
        {
            for (int file{ 1 }; file < 9; ++file)
            {
                std::cout << board.getBit(Board::pawn, Board::white, file, rank) << ' ';
            }
            std::cout << '\n';
        }
        ++it;
        std::cout << "\n\n";
    }
}
