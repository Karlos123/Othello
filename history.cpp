#include "history.hpp"
#include <list>


/* Vrati nasledujici stav desky */
const TState& History::nextState(){
  if(++it == states.end()) // Neni nasledujiciho stavu
    it--;
  return (*it);
}

/* Vrati predchozi stav desky*/
const TState& History::prevState(){
  if(it != states.begin()) // Neni predchoziho stavu
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
  if(states.end() != ++it)
      states.erase(it, states.end());
  it--;
  states.push_back({board, playerColor, blackScore, whiteScore});
  it++;
}




/**
 * Prevedeni historie na string
 * @return    Historia tahov v podobe retezca
 */
const std::string History::prepareToStore(){

  std::string res;

  std::list<TState>::iterator iter{states.begin()};
  std::list<TState>::iterator niter{states.begin()++};
  int size = iter->board.getSize();

  bool breaker = false;
  while(niter != states.end()){
      for (int x = 0; x < size; x++) {
          for (int y = 0; y < size; y++){
              if(iter->board.getStone(x, y) == NONE && (niter->board.getStone(x, y) == BLACK || niter->board.getStone(x, y) == WHITE)){
                res += static_cast<char>((x + 1)*16 + y + 1);
                breaker = true;
                break;
              }
          }
          if(breaker){
              breaker = false;
              break;
          }
      }
      iter++;
      niter++;
  }
  return res;
}


History::History(Board startBoard){
  states.push_back({startBoard.getSize(), BLACK, 2, 2});
  it = states.begin();
}
