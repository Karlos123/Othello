#include "game.hpp"
#include "game-logic.hpp"
#include "history.hpp"
#include "ai1.hpp"
#include "ai2.hpp"

#include <stdexcept>

/**
 * @breif Prepne tah na dalsiho hrace. Meni promenou playerColor
 */
void Game::nextTurn(){
  playerColor = playerColor == BLACK ? WHITE : BLACK;
}

/**
 * @breif Umisti kamen/y zadane barvy na desku
 * @param  X     Uzivatelem zadana souradnice X (radek)
 * @param  Y     Uzivatelem zadana souradnice Y (sloupec)
 * @param  board Aktulani hraci deska
 * @param  color Barva kamene/hrace na tahu
 * @return bool  Uspesne | neuspesne polozeni kamene/u
 */
bool Game::execTurnHuman(int X, int Y){
  Board nextBoard{board.getSize()}; /**< Nove rozlozeni kameu */

  try{
    gameLogic.init(X,Y, playerColor); // Inicializace herni logiky pro tah
    gameLogic.checkPos(board); // Kontrola jestli je policko vubec dostupne
    gameLogic.nextState(board, nextBoard); // Vygeneruje dalsi tah
  }
  catch(...){
    return false;
  }
  board = nextBoard; // Aktualizace desky
  gameLogic.init(0, 0, playerColor ==  BLACK ? WHITE : BLACK); // Nastaveni prohledavani pro dalshio hrace
  if(gameLogic.canTurn(board)) // Muze protivnik tahnout kamene?
    nextTurn();
  history.storeState(board, playerColor, std::make_pair(blackScore, whiteScore)); // Ulozeni stavu po tahu a hrace, ktery bude tahnout
  return true;
}

/**
 * @breig Kameny umisti umela inteligence
 */
void Game::execTurnAI(){
  Board nextBoard{board.getSize()}; /**< Nove rozlozeni kameu */
  TAI AI = getAIType();

  switch (AI) {
    case AI_AB: ai2NextState(board, nextBoard, playerColor); break; // Generovani tahu pomoci alpa-beta
    case AI_SIMPLE: ai1NextState(board, nextBoard, playerColor); break; // Generovani tahu pomoci SIMPLE algoritmu
    default: throw  std::invalid_argument("Nezname nastaveni umele inteligence");
  }
  gameLogic.init(0, 0, playerColor ==  BLACK ? WHITE : BLACK ); // Nastaveni prohledavani pro dalshio hrace
  board = nextBoard; // Aktualizace desky
  if(gameLogic.canTurn(board)) // Muze protivnik tahnout kamene?
    nextTurn();
  history.storeState(board, playerColor, std::make_pair(blackScore, whiteScore));
}

/**
 * @breig Projde herni desku od pro oba hrace a zjsiti jestli z nich alespon jeden muze hrat
 * @return bool True kdyz je mozny nejaky pohyb. False jinak
 */
bool Game::isEnd(){
  gameLogic.init(0,0, playerColor);
  // Muze tahnout prvni hrac ?
  if(gameLogic.canTurn(board))
    return false;
  // Muze tahnout druhy hrac ?
  gameLogic.init(0,0,playerColor ==  BLACK ? WHITE : BLACK );
  if(gameLogic.canTurn(board))
    return false;
  return true;
}

/**
 * Vyhodnoti jestl je nahu clovek nebo pocitac
 * @return clovek | pocitac (AI)
 */
TPlayer Game::onTurnAI(){
  if(playerColor == WHITE && playerWhite == AI)
    return AI;
  if(playerColor == BLACK && playerBlack == AI)
    return AI;
  return HUMAN;
}
