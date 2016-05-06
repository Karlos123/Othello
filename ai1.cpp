#include "board.hpp"
#include "types.hpp"
#include <iostream>


TColor rivalColorAI_1; /**< Protihracova barva kamenu slouzici jen k optimalizaci */
TColor playerColorAI_1; /**< Barva kamenu hrace na tahu slouzici jen k optimalizaci */

/**
 * @brief  Prohleda od souradnice X, Y ve smeru dirX, dirY. Funkce vraci pozice novych
 * moznych kamenu pro hrace playerColor a vrati vector souradnic novych kamenu
 * @param  board  Vychozi deska
 * @param  dirX     Smer prohledavani v ose x (radek)
 * @param  dirY     Smer prohledavani v ose y (sloupec)
 * @param   X       Souranice X od ktere se bude hledat mozny vektor kamenu
 * @param   Y       Souranice Y od ktere se bude hledat mozny vektor kamenu
 * @return          Vektor souradnic X,Y novych kamenu aktulaniho hrace
 */
TCordVec getPossibleStones(const Board& board, const int X, const int Y, const int dirX, const int dirY){
  //Policko neni prazdne
  if(board.getStone(X,Y) != NONE)
    return {};

  int i = X+dirX;
  int j = Y+dirY;
  TCordVec newStones{}; /**< Vector novych kamenu hrace ktery je na tahu */

  // Prohledavej herni pole
  while(board.inRange(i,j)){
    // Kamen na pozici neni souperuv a policko je prazdne
    if(board.getStone(i,j) != rivalColorAI_1){
      // Kamen je hrace na tahu = OK
      if(board.getStone(i,j) == playerColorAI_1)
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
 * @brief Tah pocitace je zvolen na zaklade nejevetsiho poctu zmenenych kamenu
 * @param board       Aktulani hraci deska
 * @param playerColor Hrac na tahu
 */
void ai1NextState(Board& board, const TColor playerColor){
  TCordVec bestMove{}, tmp{}, tmpv{};
  int size = board.getSize();
  playerColorAI_1 = playerColor;
  rivalColorAI_1 = playerColor == BLACK ? WHITE : BLACK; /**< Soupeova barva */

  // Prohledavani celeho herni desky
  for (int X = 0; X < size; X++) {
    for (int Y = 0; Y < size; Y++) {
      // Prohledavani okoli kamene na pozici i,j
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if(i%3-1 == 0 && j%3-1 == 0)
            continue;
          tmpv = getPossibleStones(board, X, Y, i%3-1, j%3-1);
          if(!tmpv.empty())
            tmp.insert(tmp.end(), tmpv.begin(), tmpv.end());
        }
      }
      // Nelezen delsi vektror = vice zmen kamenu
      if(tmp.size()>bestMove.size()){
        bestMove = tmp;
        bestMove.push_back(std::make_pair(X,Y));
      }
      else if(tmp.size() == bestMove.size() && tmp.size() != 0){
        int random = rand() % 2 ;
        if(random == 1){
          bestMove =  tmp;
          bestMove.push_back(std::make_pair(X,Y));
        }
        
      }
      tmp = {};
    }
  }
  // Umisteni kamenu na desku pomoci vektoru souradnic
  for(TCord i : bestMove){
    board.putStone(i.first, i.second, playerColor);
  }
}
