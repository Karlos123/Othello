#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include "types.hpp"

/**
 * @class Board
 *
 * @brief Herni plan hry Othello-reversi
 *
 * Trida Board poskytuje ctvercovy herni plan a funkce pro praci s nim. Trida neimplemetuje zadne logicke operace nad hernim planem, jako napriklad
 * generovani dalshio stavu hry apod. Herni deska je urcena svoji velikosti a polem hernich kamenu, reprezentovanych vektorem vektoru.
 */
class Board{
    int size{8}; /**< Velikost hraciho pole */
    TStones stones; /**< Kameny na desce */
  public:
    void construct(int);  /* Konstruje herni desky */

    void print() const; /* Tisk desky do CLI */

    /**
     * @brief Vlozi na pozici X, Y kamen barvy color.
     * @Warning Nekontroluje meze pole
     * @param X     Souradnice X
     * @param Y     Souradnice Y
     * @param color Barva pokladaneho kamene
     */
    void putStone(int X, int Y, TColor color){ stones[X][Y] = color; };

    /**
     * @brief Vrati barvu kamene na pozici X, Y.
     * @Warning Nekontroluje meze pole
     * @param X     Souradnice X
     * @param Y     Souradnice Y
     * @return      Barva kamene na sourdnici
     */
    TColor getStone(const int X, const int Y) const { return(stones[X][Y]); }

    bool inRange(const int X, const int Y) const; /* Zjisti jestli jsou souradnice v rozmezi desky */

    /**
     * @brief Zjisti jestli je souradnice X, Y dostupna pro polozeni kamene, policko musi byt oznaceno MARKSTONE.
     * @param  X Souradnice X
     * @param  Y  Souradnice Y
     * @return   True jestli je policko dostupne. Jinak false
     */
    bool isAvailable(int X, int Y) const { return (stones[X][Y] == MARKSTONE);  };

    /**
     * @vraci Velikost herni desky.
     * @return Velikost herni desky
     */
    int getSize() const { return size; }; /* Vrati velikost pole */

    Board(int x);

};

#endif // BOARD_HPP
