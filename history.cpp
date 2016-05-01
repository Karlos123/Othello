#include "history.hpp"
#include <list>


/* Vrati nasledujici stav desky */
const TState& History::nextState(){
  if(it == states.end()) // Neni nasledujiciho stavu
    return (*it);
  it++;
  return (*it);
}

/* Vrati predchozi stav desky*/
const TState& History::prevState(){
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
  states.push_back({board, playerColor, blackScore, whiteScore});
  it++;
}




/**
 * Prevedeni historie na string
 * @param  p1 Inteligence hrace cerny
 * @param  p2 Inteligence hrace bily
 * @return    Historie v podobe retezce
 */
const std::string History::prepareToStore(TPlayer p1, TPlayer p2, TAI aiT){
  std::string res;
  int size = begin(states)->board.getSize();

  res = p1;
  res += p2;
  res += aiT;
  res += size;
  for(TState& i: states){
    for (int X = 0; X < size; X++) {
      for (int Y = 0; Y < size; Y++){
        res += i.playerColor;
        res += i.blackScore;
        res += i.whiteScore;
        res += i.board.getStone(X, Y);
      }
    }
  }
  return res;
}


History::History(Board startBoard){
  states.push_back({startBoard.getSize(), BLACK, 2, 2});
  it = states.begin();
}
