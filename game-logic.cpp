#include <stdexcept>
#include <vector>
#include "game-logic.hpp"
#include "board.hpp"


#include <iostream>

/**
 * @breif  Zjisti jestli nastaveny hrac muze jeste tahnout, polozit kamen na nejake pole herniho planu.
 * @warning Vyzaduje inicializaci herni logiky, jinak nedefinovane chovani.
 * @param  board Prohledavana deska
 * @return bool  True kdyz hrac jeste muze uskutecnit tah. Jinak false
 */
bool GameLogic::canTurn(Board& board){
  TCordVec vecCords, tmp;
  int size = board.getSize();
  TColor stone;

  for (int X = 0; X < size; X++) {
    for (int Y = 0; Y < size; Y++) {
      stone = board.getStone(X, Y);
      if(stone != NONE && stone != MARKSTONE)
          continue;
      // Prohledavani okoli kamene na pozici i,j
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if(i%3-1 == 0 && j%3-1 == 0) // Pozice kamene + smery prohledavani je 0 a 0
            continue;
          tmp = getNewStones(board, X, Y, i%3-1, j%3-1);
          if(!tmp.empty())
            return true;
        }
      }
    }
  }
  return false;
}

/**
 * @brief Nastaveni herni logiky
 * @param a  Souradnice X (radek)
 * @param b  Souradnice Y (sloupec)
 * @param c  Barva kamenu hrace
 */
void GameLogic::init(int a, int b, TColor c){
  X = a;
  Y = b;
  playerColor = c;
}

/**
 * @breif Oznaceni moznych tahu na desce pomoci pseudo kamene MARKSTONE.
 * @note Nevyzaduje nastaveni tridnich promennych.
 * @param board Aktualni deska
 */
void GameLogic::markBoard(Board& board){
  TCordVec vecCords, tmp;
  int size = board.getSize();

  for (int X = 0; X < size; X++) {
    for (int Y = 0; Y < size; Y++) {
      if(board.getStone(X, Y) != NONE)
          continue;
      // Prohledavani okoli kamene na pozici i,j
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if(i%3-1 == 0 && j%3-1 == 0) // Pozice kamene + smery prohledavani je 0 a 0
            continue;
          tmp = getNewStones(board, X, Y, i%3-1, j%3-1);
          if(!tmp.empty())
            board.putStone(X, Y, MARKSTONE);
        }
      }
    }
  }
}

/**
 * @breif Odstrani vsechny pseudo kameny MARKSTONE z herni desky
 * @note Nevyzaduje nastaveni tridnich promennych.
 * @param board Aktualni deska urcena k odstranei tagu
 */
void GameLogic::cleanBoard(Board& board){
  int size = board.getSize();
  for (int X = 0; X < size; X++) {
    for (int Y = 0; Y < size; Y++) {
      if(board.getStone(X, Y) == MARKSTONE)
        board.putStone(X, Y, NONE);
    }
  }
}

/**
 * @breif  Prohleda od souradnice X, Y pozice novych kamenu pro aukutalniho hrace (playerColor) a vrati vektor souradnic novych kamenu
 * @param  oldBoard Vychozi deska
 * @param  dirX     Smer prohledavani v ose x (radek)
 * @param  dirY     Smer prohledavani v ose y (sloupec)
 * @return          Vektor souradnic X,Y novych kamenu aktulaniho hrace
 */
TCordVec GameLogic::getNewStones(const Board& oldBoard, const int X, const int Y, const int dirX, const int dirY){
  int i = X+dirX;
  int j = Y+dirY;
  TCordVec newStones{}; /**< Vector novych kamenu hrace kterye je na tahu */
  TColor rivalColor = playerColor == BLACK ? WHITE : BLACK; /**< Soupeova barva */

  // Prohledavej herni pole
  while(oldBoard.inRange(i,j)){
    // Kamen na pozici neni souperuv
    if(oldBoard.getStone(i,j) != rivalColor){
      // Kamen je hrace na tahu = OK
      if(oldBoard.getStone(i,j) == playerColor)
        return newStones;
      else{
        return {};
      }
    }
    newStones.push_back(std::make_pair(i,j));
    i+=dirX;
    j+=dirY;
  }
  return {}; // Vyhledavani skoncilo mimo herni plan
}

/**
 * @breif Kontroluje jestli je nastavena pozice X,Y v rozsahu desky a je  volna
 * @param board Hraci deska
 */
void GameLogic::checkPos(const Board& board) const {
  if(!board.inRange(X, Y) || !board.isAvailable(X,Y))
    throw std::out_of_range("Positon not avaible!");
}

/**
 * @breif Vygeneruje novy stav herni desky na zaklade X, Y a barvy kamene
 * @warning V pripade nemoznosti provedeni tahu vyhodi vyjimiku std::out_of_range
 * @note vyzaduje nastaveni vsech tridnich promennych.
 * @param oldBoard Deska pred polozenim novych kamenu
 * @param newBoard Deska s jiz polozeny
 */
void GameLogic::nextState(Board& board){
  TCordVec vecCords, tmp;
  // Prochazeni vsech sousednich poli
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if(i%3-1 == 0 && j%3-1 == 0) // Pozice kamene + smery prohledavani je 0 a 0
        continue;
      tmp = getNewStones(board, X, Y, i%3-1, j%3-1);
      vecCords.insert(std::end(vecCords), std::begin(tmp), std::end(tmp));
    }
  }
  // Nepodarilo se najit zadne kameny, ktere by si zahrat
  if(vecCords.empty()){
    throw std::out_of_range("Positon not avaible!");
  }
  // Umisteni kamenu na desku pomoci vektoru souradnic
  board.putStone(X, Y, playerColor );
  for(TCord i : vecCords){
    board.putStone(i.first, i.second, playerColor);
  }
}

/**
 * @breif Vypocita skore pro hrace hrajiciho za cerne i bile kameny
 * @note Nevyzaduje nastevni tridnich prommenych.
 * @param board      Aktualni deska
 * @param blackScore Refence na skore hrace hrajicho za cerne kameny
 * @param whiteScore Refence na skore hrace hrajicho za bile kameny
 */
void GameLogic::countScore(const Board& board, int& blackScore,int& whiteScore){
  int size =  board.getSize();
  blackScore = 0;
  whiteScore = 0;
  TColor stone;
  for (int X = 0; X < size; X++) {
    for (int Y = 0; Y < size; Y++) {
      stone = board.getStone(X,Y);
      if(stone == BLACK)
        blackScore++;
      else if(stone == WHITE)
        whiteScore++;
    }
  }
}
