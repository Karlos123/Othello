#ifndef HISTORY_HPP
#define HISTORY_HPP

#include <iostream>
#include <list>
#include <QByteArray>
#include "board.hpp"
#include "types.hpp"

/**
 * @struct TState
 *
 * @breif Struktura slouzici k ulozeni stavu hry
 */
typedef struct{
  Board board; /**< Herni deska, pred provedenim tahu */
  TColor playerColor; /**< Barva hrace na tahu, ktery bude v tomto tahu tahnout */
  int blackScore; /**< skore hrace hrajiciho za cerne kameny */
  int whiteScore; /**< skore hrace hrajiciho za bile kameny */
} TState;

/**
 * @class History
 *
 * @breif Posloupnost tahu a stavu provedenych ve hre
 *
 * Trida uchovava ve dvousmerne vazenamem seznamu stavy hry a poskytuje operace nad historii: ulozeni stavu, posun v historii vpred | vzad a prevadi
 * historie do ulozitelne formy (bytoveho pole).
 */
class History{
    std::list<TState> states; /**< Seznam stavu hry od pocatku az dokonce */
    std::list<TState>::iterator it{states.begin()};  /**< Ukazatel na stav v historii */
  public:
    void storeState(Board board, TColor playerColor, const int blackScore, const int whiteScore);
    const QByteArray prepareToStore();
    const TState& nextState();
    const TState& prevState();
    History(Board startBoard);
};


#endif // HISTORY_HPP
