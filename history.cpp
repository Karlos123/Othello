#include "history.hpp"
#include <list>

/* Vrati nasledujici stav desky */
const Board& History::nextState(const Board& board){
  if(it == states.end()) // Neni nasledujiciho stavu
    return board;
  it++;
  return (*it);
}

/* Vrati predchozi stav desky*/
const Board& History::prevState(const Board& board){
  if(it == states.begin()) // Neni predchoziho stavu
    return board;
  it--;
  return (*it);
}

/**
 * Ulozi stav hry
 * @param board       Aktualni rozlozeni
 * @param playerColor Hrac ktery bude tahnout
 */
void History::storeState(Board board, TColor playerColor){
  states.push_back(board);
  it++;
}
