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
    b.display();
    PositionCalculator posCal{};
    std::cout << '\n';
    std::list<Board> possiblePositions{afterNMoves(b,5,posCal)};
    int it{ 1 };
    std::cout << "Done. Total possible positions: " << possiblePositions.size() << '\n';

}
