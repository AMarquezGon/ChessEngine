#include <iostream>
#include "Board.h"
#include "PositionCalculator.h"
#include <future>
#include <semaphore>
#include <atomic>
#include <vector>
#include <thread>

//test

// Set this to your CPU's logical core count
static std::counting_semaphore<64> threadLimit(std::thread::hardware_concurrency());

std::list<Board> afterNMoves(Board board, int n, PositionCalculator& posCal, int depth = 0)
{
    if (n == 0) return { board };

    std::list<Board> firstMoves{ posCal.nextPossiblePositions(board) };

    // Only parallelize at the top levels, recurse normally below
    if (depth >= 1)
    {
        std::list<Board> result{};
        for (const Board& pos : firstMoves)
        {
            std::list<Board> branch{ afterNMoves(pos, n - 1, posCal, depth + 1) };
            result.splice(result.end(), branch);
        }
        return result;
    }

    std::vector<std::future<std::list<Board>>> futures;
    for (const Board& pos : firstMoves)
    {
        threadLimit.acquire();
        futures.push_back(std::async(std::launch::async, [pos, n, depth]()
            {
                PositionCalculator localPosCal{};
                auto result = afterNMoves(pos, n - 1, localPosCal, depth + 1);
                threadLimit.release();
                return result;
            }));
    }

    std::list<Board> result{};
    for (auto& future : futures)
    {
        std::list<Board> branch{ future.get() };
        result.splice(result.end(), branch);
    }
    return result;
}

void evaluatePositions(const Board& b, int moves)
{
    PositionCalculator posCal{};
    std::cout << "Initial position\n";
    b.display();
    std::cout << '\n';
    std::list<Board> nextPositions{ afterNMoves(b,1,posCal) };
    int totalCount{};
    for (const auto& pos : nextPositions)
    {
        std::cout << "Current position:\n";
        pos.display();
        std::list<Board> possiblePositions{ afterNMoves(pos,moves-1,posCal) };
        std::cout << "Total possible positions after this one: " << possiblePositions.size() << "\n\n\n";
        totalCount += possiblePositions.size();
    }

    std::cout << "Total count: " << totalCount;
}

std::int64_t perft(Board board, int depth, PositionCalculator& posCal, int threadDepth = 0)
{
    if (depth == 0) return 1;

    std::list<Board> moves{ posCal.nextPossiblePositions(board) };

    if (depth == 1) return moves.size();

    if (threadDepth >= 1)
    {
        std::int64_t count{ 0 };
        for (const Board& pos : moves)
            count += perft(pos, depth - 1, posCal, threadDepth + 1);
        return count;
    }

    std::vector<std::future<std::int64_t>> futures;
    for (const Board& pos : moves)
    {
        futures.push_back(std::async(std::launch::async, [pos, depth, threadDepth]()
            {
                PositionCalculator localPosCal{};
                return perft(pos, depth - 1, localPosCal, threadDepth + 1);
            }));
    }

    std::int64_t count{ 0 };
    for (auto& future : futures)
        count += future.get();

    return count;
}

int main()
{

    Board b{};
    PositionCalculator posCal{};
    //b.eliminatePiece(1ULL << 12); //eliminate e2
    //b.m_pieces[Board::pawn] |= (1ULL << 20); // add e3
    ////b.m_currentTurn = Board::black;
    //b.eliminatePiece(1ULL << 52); // eliminate e7
    //b.m_pieces[Board::pawn + Board::max_pieces] |= (1ULL << (36)); // add e5
    //b.eliminatePiece(1ULL << 11); //eliminate d2
    //b.m_pieces[Board::pawn] |= (1ULL << 27); // add d4
    //b.eliminatePiece(shift(1ULL << (36), 9, Board::black));
    //b.m_pieces[Board::pawn + Board::max_pieces * Board::black] = (b.m_pieces[Board::pawn + Board::max_pieces * Board::black] & (~(1ULL << (36)))) | shift(1ULL << (36), 9, Board::black);
    //b.eliminatePiece(1ULL << 36); // eliminate e5
    //b.m_pieces[Board::pawn + Board::max_pieces] |= (1ULL << (36)); // add e5


    //Board castleTest{};
    //castleTest.m_pieces[Board::bishop+Board::max_pieces] = 0;
    //castleTest.m_pieces[Board::knight + Board::max_pieces] = 0;
    //castleTest.m_pieces[Board::queen + Board::max_pieces] = 0;
    //castleTest.m_currentTurn = Board::black;
    //castleTest.display();
    //std::list<Board> possiblePositions{ afterNMoves(castleTest,1,posCal) };
    //for (const auto& pos : possiblePositions)
    //{
    //    pos.display();
    //    std::cout << '\n';
    //}
    

    //std::list<Board> possiblePositions{ afterNMoves(b, 4, posCal) };
    //std::int64_t totalCount{};
    //std::atomic<int> completed{ 0 };
    //int total{ (int)possiblePositions.size() };

    //for (const auto& pos : possiblePositions)
    //{
    //    std::list<Board> nextPos{ afterNMoves(pos, 3, posCal) };
    //    totalCount += nextPos.size();

    //    int done = ++completed;
    //    if (done % 1000 == 0 || done == total) // print every 100 iterations
    //    {
    //        std::cout << "Progress: " << done << "/" << total
    //            << " (" << (100 * done / total) << "%)"
    //            << " | Running total: " << totalCount
    //            << "\n";
    //    }
    //}

    std::cout << "Total possible positions: " << perft(b,7,posCal) << "\n";

    //std::cout << '\n';
    //std::list<Board> possiblePositions{ afterNMoves(b,3,posCal) };

    //std::cout << "Done. Total possible positions for: " << possiblePositions.size() << "\n\n\n";
    

}
