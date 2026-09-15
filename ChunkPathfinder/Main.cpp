#include <iostream>
#include <random>
#include <vector>
#include <queue>
#include <array>
#include <iomanip>

#include "Types.hpp"

using namespace Pathos;

constexpr Chunk TestChunk = {
    // y = 0
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 1
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 2
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 3
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 4
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 5
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 6
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 7 (drop row)
    Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop, Tile::SouthDrop,
    // y = 8
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 9
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 10
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 11
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 12
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 13
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 14
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
    // y = 15
    Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open, Tile::Open,
};
int main()
{

    std::vector<Portal> Portals = TestChunk.GetPortals();

    return 0;
}