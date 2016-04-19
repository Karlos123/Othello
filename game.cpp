#include "game.hpp"
#include "game-logic.hpp"
#include "history.hpp"


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
bool Game::execTurn(int X, int Y, Board& board){
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
  gameLogic.init(0, 0, playerColor ==  BLACK ? WHITE : BLACK ); // Nastaveni prohledavani pro dalshio hrace
  if(gameLogic.canTurn(board)) // Muze protivnik tahnout kamene?
    nextTurn();
  return true;
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
