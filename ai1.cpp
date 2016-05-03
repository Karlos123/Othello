#include "board.hpp"
#include "types.hpp"
#include <iostream>

/**
 * @breif  Prohleda od souradnice X, Y ve smeru dirX, dirY. Funkce vraci pozice novych
 * moznych kamenu pro hrace playerColor a vrati vector souradnic novych kamenu
 * @param  oldBoard Vychozi deska
 * @param  dirX     Smer prohledavani v ose x (radek)
 * @param  dirY     Smer prohledavani v ose y (sloupec)
 * @param  playerColor Barva hrace na tahu
 * @return          Vektor souradnic X,Y novych kamenu aktulaniho hrace
 */
TCordVec getPossibleStones(const Board& oldBoard, const int X, const int Y, const int dirX, const int dirY, TColor playerColor){
  int i = X+dirX;
  int j = Y+dirY;
  TCordVec newStones{}; /**< Vector novych kamenu hrace ktery je na tahu */
  TColor rivalColor = playerColor == BLACK ? WHITE : BLACK; /**< Soupeova barva */

  //Policko neni prazdne
  if(oldBoard.getStone(X,Y) != NONE)
    return {};
  // Prohledavej herni pole
  while(oldBoard.inRange(i,j)){
    // Kamen na pozici neni souperuv a policko je prazdne
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
 * Tah pocitace je zvolen na zaklade nejevetsiho poctu zmenenych kamenu
 * @param board       Aktulani hraci deska
 * @param nextBoard   Nova herni deska
 * @param playerColor Hrac na tahu
 */
void ai1NextState(const Board& oldBoard, Board& newBoard, const TColor playerColor){
  TCordVec bestMove{}, tmp{}, tmpv{};
  int size = oldBoard.getSize();
  // Prohledavani celeho herni desky
  for (int X = 0; X < size; X++) {
    for (int Y = 0; Y < size; Y++) {
      // Prohledavani okoli kamene na pozici i,j
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if(i%3-1 == 0 && j%3-1 == 0)
            continue;
          tmpv = getPossibleStones(oldBoard, X, Y, i%3-1, j%3-1, playerColor);
          if(!tmpv.empty())
            tmp.insert(tmp.end(), tmpv.begin(), tmpv.end());
        }
      }
      // Nelezen delsi vektror = vice zmen kamenu
      if(tmp.size()>bestMove.size()){
        bestMove = tmp;
        bestMove.push_back(std::make_pair(X,Y));
      }
      if(tmp.size() == bestMove.size() && tmp.size() != 0){
        int random = rand() % 2 ;
        if(random == 1)
          bestMove =  tmp;
        bestMove.push_back(std::make_pair(X,Y));
      }
      tmp = {};
    }
  }
  // Umisteni kamenu na desku pomoci vektoru souradnic
  newBoard = oldBoard;
  for(TCord i : bestMove){
    newBoard.putStone(i.first, i.second, playerColor);
  }
}
