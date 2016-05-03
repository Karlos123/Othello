#include "game.hpp"
#include "game-logic.hpp"
#include "history.hpp"
#include "ai1.hpp"
#include "ai2.hpp"

#include <stdexcept>
#include <QFile>

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
    gameLogic.cleanBoard(board);
    gameLogic.nextState(board, nextBoard); // Vygeneruje dalsi tah
  }
  catch(...){
    return false;
  }
  board = nextBoard; // Aktualizace desky
  gameLogic.init(0, 0, playerColor ==  BLACK ? WHITE : BLACK); // Nastaveni prohledavani pro dalshio hrace
  if(gameLogic.canTurn(board)) // Muze protivnik tahnout kamene?
    nextTurn();
  gameLogic.init(0, 0, playerColor); // Nastaveni prohledavani pro dalshio hrace
  if(onTurnAI() != AI)
    gameLogic.markBoard(board);
  gameLogic.countScore(board, blackScore, whiteScore);
  history.storeState(board, playerColor, blackScore, whiteScore); // Ulozeni stavu po tahu a hrace, ktery bude tahnout
  return true;
}

/**
 * @breig Kameny umisti umela inteligence
 */
void Game::execTurnAI(){
  Board nextBoard{board.getSize()}; /**< Nove rozlozeni kameu */
  TAI AI = getAIType();
  gameLogic.cleanBoard(board);
  switch (AI) {
    case AI_AB: ai2NextState(board, nextBoard, playerColor); break; // Generovani tahu pomoci alpa-beta
    case AI_SIMPLE: ai1NextState(board, nextBoard, playerColor); break; // Generovani tahu pomoci SIMPLE algoritmu
    default: throw  std::invalid_argument("Nezname nastaveni umele inteligence");
  }
  gameLogic.init(0, 0, playerColor ==  BLACK ? WHITE : BLACK ); // Nastaveni prohledavani pro dalshio hrace
  board = nextBoard; // Aktualizace desky
  if(gameLogic.canTurn(board)) // Muze protivnik tahnout kamene?
    nextTurn();
  gameLogic.init(0, 0, playerColor); // Nastaveni prohledavani pro dalshio hrace
  if(onTurnAI() != AI)
    gameLogic.markBoard(board);
  gameLogic.countScore(board, blackScore, whiteScore);
  history.storeState(board, playerColor, blackScore, whiteScore);
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

/**
 * Ulozi rozohranu hru do suboru s nazvom FileName
 * @return 0 ak sa operacia vydarila, nenulova hodnota v pripade chyby
 */
int Game::saveGame(QString fileName){
    QByteArray save;
    save.append(board.getSize());
    if(isPvEGame()) // vlozit podmienku, ze ci je druhy hrac AI
        save.append(static_cast<uchar>(getAIType()) + 1);
    else
        save.append(static_cast<char>(0));

    // Pridanie postupnosti vykonanych tahov do QByteArray - preiterovat cez historiu

    save.append(QString::fromStdString(history.prepareToStore()));

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
        return 1;

    file.write(save);
    file.close();

    return 0;
}

/**
 * Nacita rozohranu hru zo suboru s nazvom FileName
 * @return bajtove pole ulozenej hry ak sa operacia vydarila, prazdne bajtove pole v pripade chyby
 */
QByteArray Game::loadGame(QString fileName){
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return "";

    QByteArray save = file.readAll();
    file.close();

    // Kontrola Save Game suboru

    // Kontrola velkosti: +2 znaky kvoli informaciam o velkosti pola a AI, -4 znaky kvoli pociatocnym 4 kamenom
    if(save.length() > save.at(0)*save.at(0) - 2)
        return "";

    // Kontrola, ci policka su v platnom rozsahu hodnot
    for(uchar i = 2; i < save.length(); i++)
        if(save.at(i)/16 > save.at(0) || save.at(i)%16 > save.at(0) || !((save.at(i)/16)*(save.at(i)%16)))
            return "";

    // Validita tahov sa uz asi nebude overovat...

    return save;
}
