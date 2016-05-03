#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "types.hpp"


class Board{
    int size{8}; // Velikost hraciho pole
    TStones stones; // Kameny na desce
  public:
    void construct(int);
    void print() const;
    void draw() const;
    void putStone(int X, int Y, TColor color){ stones[X][Y] = color; };
    TColor getStone(const int X, const int Y) const { return(stones[X][Y]); }
    bool inRange(const int X, const int Y) const;
    bool isAvailable(int X, int Y) const { return (stones[X][Y] == MARKSTONE);  };
    int getSize() const { return size; };
    Board(int x);

};

#endif // BOARD_HPP
