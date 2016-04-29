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
 * @breif Ulozi stav hry
 * @param board       Aktualni rozlozeni
 * @param playerColor Hrac ktery bude tahnout
 */
void History::storeState(Board board, TColor playerColor, const int blackScore, const int whiteScore){
  // Odstraneni vsech ulozenych stavu ktere jsou az za aktualni pozici v historii
  if(states.end() != it)
      states.erase(it++,states.end()++ );
  /*for(auto i = states.end(); it != i; i--)
    states.erase(i);*/
  states.push_back({board, playerColor, blackScore, whiteScore});
  it++;
}


History::History(Board startBoard){
  states.push_back({startBoard.getSize(), BLACK, 2, 2});
  it = states.begin();
}
