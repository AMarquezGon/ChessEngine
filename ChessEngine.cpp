#include <iostream>
#include "Board.h"

int main()
{
    Board b{};
    for (int rank{ 8 }; rank > 0; --rank)
    {
        for (int file{ 8 }; file > 0; --file)
        {
            std::cout << b.getBit(Board::bishop, Board::black, file, rank) << ' ';
        }
        std::cout << '\n';
    }
}
