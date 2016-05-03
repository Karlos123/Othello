#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "types.hpp"


class Board{
    int size{8}; /**< Velikost hraciho pole */
    TStones stones; /**< Kameny na desce */
  public:
    void construct(int);  /* Konstruje herni desky */
    void print() const; /* Tisk desky do CLI */
    void putStone(int X, int Y, TColor color){ stones[X][Y] = color; }; /* Polozeni kamenu dane barvy na desku */
    TColor getStone(const int X, const int Y) const { return(stones[X][Y]); } /* Vraci kamen na zadane pozici */
    bool inRange(const int X, const int Y) const; /* Zjisti jestli jsou souradnice v rozmezi desky */
    bool isAvailable(int X, int Y) const { return (stones[X][Y] == MARKSTONE);  }; /* Zjisti jestli je na souradnici X, Y mozno polozit kamen */
    int getSize() const { return size; }; /* Vrati velikost pole */
    Board(int x);

};

#endif // BOARD_HPP
