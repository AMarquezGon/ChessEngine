#include <iostream>
#include "Board.h"
#include "PositionCalculator.h"

std::list<Board> afterNMoves(Board board, int n, PositionCalculator& posCal)
{
    std::list<Board> currentPositions{ board };
    for (int i{ 0 }; i < n; ++i)
    {
        std::list<Board> nextPositions{};
        while (!currentPositions.empty())
        {
            std::list<Board> futurePosition{ posCal.nextPossiblePositions(currentPositions.front()) };
            nextPositions.insert(nextPositions.end(), futurePosition.begin(), futurePosition.end());
            currentPositions.pop_front();
        }
        currentPositions = nextPositions;
    }
    return currentPositions;
}

int main()
{
    Board b{};
    b.m_pieces[Board::queen + Board::max_pieces] = 1ULL << 18;
    //b.m_pieces[Board::pawn] = 1ULL << 50;
    //b.m_pieces[Board::pawn + Board::max_pieces] = 1ULL << 36;
    //b.m_enpassant = 1 << 4;
    b.display();
    PositionCalculator posCal{};
    std::cout << '\n';
    //std::list<Board> possiblePositions{b.nextPossiblePositions()};
    std::list<Board> possiblePositions{afterNMoves(b,1,posCal)};
    int it{ 1 };
    std::cout << "Done";
    for (const Board& board : possiblePositions)
    {
        //std::cout << board.getCurrentTurn();
        std::cout << "Board number " << it << '\n';
        board.display();
        ++it;
        std::cout << "\n\n";
    }
}
