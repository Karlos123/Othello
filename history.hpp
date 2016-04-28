#ifndef HISTORY_HPP
#define HISTORY_HPP

#include <iostream>
#include <list>
#include "board.hpp"
#include "types.hpp"

typedef struct{
  Board board;
  TColor playerColor;
  int blackScore, whiteScore;
} TState;

class History{
    std::list<TState> states;
    std::list<TState>::iterator it{states.begin()};
  public:
    void storeState(Board board, TColor playerColor, std::pair<int, int> score);
    const TState& nextState(Board const&);
    const TState& prevState(Board const&);
    History(Board startBoard);
};


#endif // HISTORY_HPP
