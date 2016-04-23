#ifndef HISTORY_HPP
#define HISTORY_HPP

#include <iostream>
#include <list>
#include "board.hpp"
#include "types.hpp"

class History{
    std::list<Board> states;
    std::list<Board>::iterator it{states.begin()};
  public:
    void storeState(Board board, TColor);
    const Board& nextState(Board const&);
    const Board& prevState(Board const&);
};


#endif // HISTORY_HPP
