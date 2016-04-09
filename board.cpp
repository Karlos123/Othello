#include "board.hpp"
#include <iostream>
#include <iomanip>


/* Tisk hraciho pole (CLI)  */
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
        case CNone:
          std::cout <<  " ";
          break;
        case CBlack:
          std::cout << "\u25CB";
          break;
        case CWhite:
          std::cout << "\u25CF";
          break;
      }
      std::cout << " ";
    }
    std::cout << std::endl << std::setw(2);
  }
  std::cout << std::endl;
}

/* Vykresleni hraciho pole  */
void Board::draw() const {
}

/* Konstrukce pole - v pocatecnim stavu */
void Board::construct(int size){
  Board::size = size;
  // Konstrukce hraciho pole
  for (int i = 0; i < size; i++) {
    std::vector<COLOR> row (Board::size, CNone);
    stones.push_back(row);
  }
  // Umisteni kamenu na desku
  stones[size/2-1][size/2-1] = CWhite;
  stones[size/2-1][size/2] = CBlack;
  stones[size/2][size/2-1] = CBlack;
  stones[size/2][size/2] = CWhite;
}
