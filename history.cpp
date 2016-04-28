#include "history.hpp"
#include <list>

/* Vrati nasledujici stav desky */
const TState& History::nextState(const Board& board){
  if(it == states.end()) // Neni nasledujiciho stavu
    return (*it);
  it++;
  return (*it);
}

/* Vrati predchozi stav desky*/
const TState& History::prevState(const Board& board){
  if(it == states.begin()) // Neni predchoziho stavu
    return (*it);
  it--;
  return (*it);
}

/**
 * Ulozi stav hry
 * @param board       Aktualni rozlozeni
 * @param playerColor Hrac ktery bude tahnout
 */
void History::storeState(Board board, TColor playerColor, std::pair<int, int> score){
  TState tmp{board.getSize(), playerColor, score.first, score.second};
  // Odstraneni vsech ulozenych stavu ktere jsou az za aktualni pozici v historii
  for(auto i = states.end(); it != i; i--)
    states.erase(i);
  states.push_back(tmp);
  it++;
}


History::History(Board startBoard){
  TState tmp = {startBoard.getSize(), BLACK, 2, 2};
  states.push_back(tmp);
}
