#include "ai2.hpp"
#include "board.hpp"


#include <cstdlib>


TColor rivalColorAI_2, playerColorAI_2;

/**
 * @brief  Prohleda od souradnice X, Y ve smeru dirX, dirY. Funkce vraci pozice novych
 * moznych kamenu pro hrace playerColor a vrati vektor souradnic novych kamenu
 * @param  board  Vychozi deska
 * @param   X       Souranice X od ktere se bude hledat mozny vektor kamenu
 * @param   Y       Souranice Y od ktere se bude hledat mozny vektor kamenu
 * @param  dirX     Smer prohledavani v ose x (radek)
 * @param  dirY     Smer prohledavani v ose y (sloupec)
 * @return          Vektor souradnic X,Y novych kamenu aktulaniho hrace
 */
TCordVec findPossibleStones(const Board& board, const int X, const int Y, const int dirX, const int dirY){
  //Policko neni prazdne
  if(board.getStone(X,Y) != NONE)
    return {};

  int i = X+dirX;
  int j = Y+dirY;
  TCordVec newStones{}; /**< Vector novych kamenu hrace ktery je na tahu */

  // Prohledavej herni pole
  while(board.inRange(i,j)){
    // Kamen na pozici neni souperuv a policko je prazdne
    if(board.getStone(i,j) != rivalColorAI_2){
      // Kamen je hrace na tahu = OK
      if(board.getStone(i,j) == playerColorAI_2)
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
 * @param playerColor Hrac na tahu
 */
void ai2NextState(Board& board, const TColor playerColor){
  TCordVec tmp{}, tmpv{};
  int size = board.getSize();
  std::vector<TCordVec> moveVec;
  playerColorAI_2 = playerColor;
  rivalColorAI_2 = playerColor == BLACK ? WHITE : BLACK; /**< Soupeova barva */

  // Prohledavani celeho herni desky
  for (int X = 0; X < size; X++) {
    for (int Y = 0; Y < size; Y++) {
      // Prohledavani okoli kamene na pozici i,j
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if(i%3-1 == 0 && j%3-1 == 0)
            continue;
          tmpv = findPossibleStones(board, X, Y, i%3-1, j%3-1);
          if(!tmpv.empty())
            tmp.insert(tmp.end(), tmpv.begin(), tmpv.end());
        }
      }
      if(tmp.size() > 0){
        tmp.push_back(std::make_pair(X,Y));
        moveVec.push_back(tmp);
        tmp = {};
      }
    }
  }

  int random = rand() % moveVec.size() ;
  // Umisteni kamenu na desku pomoci vektoru souradnic
  for(TCord i : moveVec[random]){
    board.putStone(i.first, i.second, playerColor);
  }
}
