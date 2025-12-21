#include <iostream>
#include "Board.h"


std::list<Board> afterNMoves(Board board, int n)
{
    std::list<Board> currentPositions{ board };
    for (int i{ 0 }; i < n; ++i)
    {
        std::list<Board> nextPositions{};
        while (!currentPositions.empty())
        {
            std::list<Board> futurePosition{ currentPositions.front().nextPossiblePositions() };
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
    //b.m_pieces[Board::queen + Board::max_pieces] = 1ULL << 19;
    //b.m_pieces[Board::pawn] = 1ULL << 50;
    //b.m_pieces[Board::pawn + Board::max_pieces] = 1ULL << 36;
    //b.m_enpassant = 1 << 4;
    b.display();
    std::cout << '\n';
    //std::list<Board> possiblePositions{b.nextPossiblePositions()};
    std::list<Board> possiblePositions{afterNMoves(b,3)};
    int it{ 1 };

    /*for (Board board : possiblePositions)
    {
        std::list<Board> nextpossiblePositions{ board.nextPossiblePositions() };
        for (Board board : nextpossiblePositions)
        {
            std::cout << "Board number " << it << '\n';
            board.display();
            ++it;
            std::cout << "\n\n";
        }
    }*/
    std::cout << "Done";
    for (Board board : possiblePositions)
    {
        //std::cout << board.getCurrentTurn();
        std::cout << "Board number " << it << '\n';
        board.display();
        ++it;
        std::cout << "\n\n";
    }
}
