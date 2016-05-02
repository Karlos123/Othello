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
    void storeState(Board board, TColor playerColor, const int blackScore, const int whiteScore);
    const std::string prepareToStore();
    const TState& nextState();
    const TState& prevState();
    History(Board startBoard);
};


#endif // HISTORY_HPP
