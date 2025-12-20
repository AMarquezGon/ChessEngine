#include <iostream>
#include "Board.h"

int main()
{
    Board b{};
    b.m_pieces[Board::queen + Board::max_pieces] = 1ULL << 19;
    b.m_pieces[Board::pawn] = 1ULL << 35;
    b.m_enpassant = 1 << 4;
    b.display();
    std::list<Board> possiblePositions{b.nextPossiblePositions(Board::white)};
    int it{ 1 };

    for (Board board : possiblePositions)
    {
        std::cout << "Board number " << it << '\n';
        board.display();
        ++it;
        std::cout << "\n\n";
    }
}
