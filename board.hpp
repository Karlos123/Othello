#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>

enum COLOR { CNone, CBlack, CWhite}; // Typ kamenu

class Board{
    int size{8}; // Velikost hraciho pole
    std::vector<std::vector<COLOR>> stones; // Kameny na desce
  public:
    void construct(int);
    void print() const;
    void draw() const;
    void putStone(int X, int Y, COLOR color){stones[X][Y] = color;  };
    bool isEmpty(int X, int Y) { return (stones[X][Y] == CNone);  };
    int isSize() const { return size; };
};

#endif // BOARD_HPP
