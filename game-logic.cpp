#include <stdexcept>
#include <vector>
#include "game-logic.hpp"
#include "board.hpp"


#include <iostream>

/**
 * @breif  Zjisti jestli hrac (playerColor) muze polozit kamen.
 * @param  board Prohledavana deska
 * @param  color Barva hrace pro kterehho se prohledava
 * @return bool  True kdyz je mozne jeste pohyb. Jiank false
 */
bool GameLogic::canTurn(const Board& board){
  TCordVec vecCords, tmp;

  // Prohledavani celeho herni desky
  for (int X = 0; X < board.getSize(); X++) {
    for (int Y = 0; Y < board.getSize(); Y++) {
      // Prohledavani okoli kamene na pozici i,j
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
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
 * Nastaveni herni logiky
 * @param a  Uzivatelem zadana souradnice X (radek)
 * @param b  Uzivatelem zadana souradnice Y (sloupec )
 * @param c  Barva kamenu aktualniho hrace
 */
void GameLogic::init(int a, int b, TColor c){
  X = a;
  Y = b;
  playerColor = c;
}

/**
 * @breif  Prohleda od souradnice X, Y pozice novych kamenu pro aukutalniho hrace (playerColor) a vrati vector souradnic novych kamenu
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
 * @breif Kontorluje jestli je nastavena pozice X,Y v rozsahu deskz a je  volna
 * @param board Hraci deska
 */
void GameLogic::checkPos(const Board& board) const {
  if(!board.inRange(X, Y) || !board.isEmpty(X,Y))
    throw std::out_of_range("Positon not avaible!");
}

/**
 * @breif Vygeneruje novy stav herni desky na zaklade X, Y a barvy kamene
 * @param oldBoard Deska pred polozenim novych kamenu
 * @param newBoard Deska s jiz polozeny
 */
void GameLogic::nextState(const Board& oldBoard, Board& newBoard){
  TCordVec vecCords, tmp;
  // Prochazeni vsech sousednich poli
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      tmp = getNewStones(oldBoard, X, Y, i%3-1, j%3-1);
      vecCords.insert(std::end(vecCords), std::begin(tmp), std::end(tmp));
    }
  }
  // Nepodarilo se najit zadne kameny, ktere by si zahrat
  if(vecCords.empty())
    throw std::out_of_range("Positon not avaible!");
  // Umisteni kamenu na desku pomoci vektoru souradnic
  newBoard = oldBoard;
  newBoard.putStone(X, Y, playerColor );
  for(TCord i : vecCords){
    newBoard.putStone(i.first, i.second, playerColor);
  }

}
