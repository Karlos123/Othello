#include "board.hpp"
#include <iostream>
#include <iomanip>


Board::Board(int x){
  size = x;
  construct(x);
}

/**
 * @breif Tisk hraciho desky do konzole
 */
void Board::print() const {
  // Tisk horniho ocislovani hraciho pole
  std::cout << std::setw(4);
  for (int i = 0; i < Board::size; i++) {
    std::cout << char(i+97) << ' ';
  }
  std::cout<< std::endl << std::setw(2);
  // Tisk herniho planu a kamenu
  for (int i = 0; i < Board::size; i++) {
    std::cout<< i+1 << " ";
    for (int j = 0; j < Board::size; j++) {
      switch (stones[i][j]){ // typ kamene na policku
        case MARKSTONE:
          std::cout <<  "!";
          break;
        case NONE:
          std::cout <<  " ";
          break;
        case BLACK:
          std::cout << "O";
          break;
        case WHITE:
          std::cout << "@";
          break;
      }
      std::cout << " ";
    }
    std::cout<< i+1;
    std::cout << std::endl << std::setw(2);
  }
  // Tisk spodniho ocislovani
  std::cout << std::setw(4);
  for (int i = 0; i < Board::size; i++) {
    std::cout << char(i+97) << ' ';
  }
  std::cout << std::endl;
}

/**
 * Vytvori ctvercovou hraci desku o zadane velikost
 * @param size Pozadovana velikost desky
 */
void Board::construct(int size){
  Board::size = size;
  // Konstrukce hraciho pole
  for (int i = 0; i < size; i++) {
    std::vector<TColor> row (Board::size, NONE);
    stones.push_back(row);
  }
  // Umisteni kamenu na desku
  stones[size/2-1][size/2-1] = WHITE;
  stones[size/2-1][size/2] = BLACK;
  stones[size/2][size/2-1] = BLACK;
  stones[size/2][size/2] = WHITE;
  stones[size/2][size/2+1] = MARKSTONE;
  stones[size/2+1][size/2] = MARKSTONE;
  stones[size/2-2][size/2-1] = MARKSTONE;
  stones[size/2-1][size/2-2] = MARKSTONE;
}

/**
 * @breif Zjisti jestli jsou souradnice v rozmezi desky
 * @param  X Souradnice Xs
 * @param  Y Souradnice Y
 * @return   bool True jestli je v rozmezi
 */
bool Board::inRange(const int X, const int Y) const{
  return(X >= 0 && Y >= 0 && X <= size-1 && Y <= size-1);
}
