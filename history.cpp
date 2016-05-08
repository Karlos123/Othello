#include "history.hpp"
#include <list>


/**
 * @brief Vrati nasledujici stav v historii a posune ukazatel na stav na tento stav.
 * @note Kdyz neni dalsiho stavu v historii vrati aktualni stav historie
 * @return Dalsi stav hry
 */
const TState& History::nextState(){
  if(++it == states.end()) // Neni nasledujiciho stavu
    it--;
  return (*it);
}

/**
 * @brief Vrati predchozi stav v historii a posune ukazatel na stav na tento stav.
 * @note Kdyz neni predchozi stav v historii vrati aktualni stav.
 * @return Predchozi stav hry
 */
const TState& History::prevState(){
  if(it != states.begin()) // Neni predchoziho stavu
    it--;
  return (*it);
}

/**
 * @brief Ulozi stav hry do obousmerne vazaneho seznamu historie
 * @note Jestlize se ukazatel stav nenachazi na poslednim stavu (byl proveden alespon jeden posun zpet v historii) odstrani vsechy
 * zaznamy od aktulaniho stavu az po posledni stav v historii z historie.
 * @param board       Aktualni herni deska
 * @param playerColor Hrac na tahu
 * @param blackScore  Skore (pocet kamenu) hrace za cerne kameny
 * @param whiteScore  Skore (pocet kamenu) hrace za bile kameny
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
 * @brief Pripravi historii k ulozeni. Prevede stavy z historie na bytove pole.
 * @return  Historia tahov v podobe bytoveho pole
 */
const QByteArray History::prepareToStore(){

  QByteArray res;

  std::list<TState>::iterator iter{states.begin()};
  std::list<TState>::iterator niter{states.begin()};
  niter++;
  int size = iter->board.getSize();

  bool breaker = false;
  while(niter != states.end()){
    for (int x = 0; x < size; x++) {
      for (int y = 0; y < size; y++){
        if((iter->board.getStone(x, y) == NONE || iter->board.getStone(x, y) == MARKSTONE) &&
            (niter->board.getStone(x, y) == BLACK || niter->board.getStone(x, y) == WHITE)){
          res += static_cast<unsigned char>((x + 1)*16 + y + 1);
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

/**
 * @brief Konstruktor historie ulozi vychozi stav hry do historie a nastavi ukazatel na stav na tento stav.
 * @param startBoard Vychozi stav hry
 */
History::History(Board startBoard){
  states.push_back({startBoard.getSize(), BLACK, 2, 2});
  it = states.begin();
}
