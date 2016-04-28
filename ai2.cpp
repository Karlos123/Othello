#include "ai2.hpp"
#include "board.hpp"

#include <algorithm>
#include <limits>
#include <cstdlib>
#include <iostream>


const int MaxDepth = 6; /**< Maximalni hloubka prohledavani */
int size; /**< velikost herniho planu - optimalizace */
TColor curPlayer; /**< Aktualni hrac na tahu */

/**
 * @breif  Prohleda od souradnice X, Y pozice novych kamenu pro aukutalniho hrace (playerColor) a vrati vector souradnic novych kamenu
 * @param  oldBoard Vychozi deska
 * @param  dirX     Smer prohledavani v ose x (radek)
 * @param  dirY     Smer prohledavani v ose y (sloupec)
 * @param  playerColor Barva hrace na tahu
 * @return          Vektor souradnic X,Y novych kamenu aktulaniho hrace
 */
TCordVec findNewVec(const Board& oldBoard, const int X, const int Y, const int dirX, const int dirY, TColor playerColor){
    //Policko neni prazdne
  if(oldBoard.getStone(X,Y) != NONE)
    return {};

  int i = X+dirX;
  int j = Y+dirY;
  TCordVec newStones{}; /**< Vector novych kamenu hrace ktery je na tahu */
  TColor rivalColor = playerColor == BLACK ? WHITE : BLACK; /**< Soupeova barva */

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
 * @breif Nalezeni vsech moznych tahu
 * @param possibleMoves Vektor moznych tahu
 * @param oldBoard      Vychozi deska
 * @param playerColor   Barva hrace pro ktereho jsou vyhledavany vektory tahu
 */
void findPossibleMoves(std::vector<Board>& possibleMoves, const Board& oldBoard, const TColor playerColor){
  TCordVec newStones{};
  Board possibleMove{size};

  // Prohledavani celeho herni desky
  for (int X = 0; X < size; X++) {
    for (int Y = 0; Y < size; Y++) {
      // Prohledavani okoli kamene na pozici i,j
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if(i%3-1 == 0 && j%3-1 == 0)
            continue;
          newStones = findNewVec(oldBoard, X, Y, i%3-1, j%3-1, playerColor); // Nalezeni noveho vektoru kamenu (souradnic)
          if(!newStones.empty()){
            possibleMove = oldBoard; // Inicializace vychozihi desky
            newStones.push_back(std::make_pair(X,Y)); // Pridani pokladaneho kamene do vektoru
            for(const TCord& i : newStones) // Umisteni kamenu na desku
              possibleMove.putStone(i.first, i.second, playerColor);
            possibleMoves.push_back(possibleMove); // Ulozeni mozneho rozlozeni
          }
        }
      }
    }
  }
}

/**
 * @breif Ohodnoceni aktulaniho stavu herni desky
 * @param  board Aktualni herni deska
 * @param  max   True - vybirame pro aktulaniho hrace | False - vybirame pro protihrace
 * @return       Ohodnoceni
 */
int evaluate(const Board& board){
    TColor stone;
    int blackScore = 0;
    int whiteScore = 0;

    for (int X = 0; X < size; X++) {
      for (int Y = 0; Y < size; Y++) {
        stone = board.getStone(X, Y);
        if(stone == BLACK)
          blackScore++;
        else if(stone == WHITE)
          whiteScore++;
      }
    }
    return curPlayer == BLACK ? blackScore - whiteScore : whiteScore - blackScore;
}



/**
 * @breif Prohledavani stavoveho prostoru pomoci min-max a alpha-beta
 * @param  board Vychozi deska
 * @param  depth Akualni hloubka zanoreni
 * @param  a     Hodnota alpha
 * @param  b     Hodnota beta
 * @param  max   True - vybirame maximum | mininum
 * @return       Alpha | beta
 */
int minimax(const Board& board, int depth, int a, int b, bool max){
    // Dosazeno max hloubky = ohodnoceni stavu
    if (depth == 0)
        return evaluate(board);
    // Generovani dalsich moznych stavu
    std::vector<Board> possibleMoves;
    findPossibleMoves(possibleMoves, board, max ? WHITE : BLACK);
    // Bereme max
    if (max) {
        for (const Board &move : possibleMoves) {
            a = std::max(a, minimax(move, depth - 1, a, b, false));
            if (b <= a)
                return b; // β cutoff.
        }
        return a;
    // Bereme min
    } else {
        for (const Board &move : possibleMoves) {
            b = std::min(b, minimax(move, depth - 1, a, b, true));
            if (b <= a)
                return a; // α cutoff.
        }
        return b;
    }
}

/**
 * Nalezeni dalsiho tahu pomoci min-max doplneneho o alpha-betu
 * @param board       Aktualni stav herni desky
 * @param newBoard    Nove rozlozeni herni desky
 * @param playerColor Barva hrace na tahu
 */
void ai2NextState(const Board& board,  Board& newBoard,  TColor playerColor){
    size = board.getSize();
    std::vector<Board> possibleMoves;
    findPossibleMoves(possibleMoves, board, playerColor);

    int a = -std::numeric_limits<int>::max();
    int b = std::numeric_limits<int>::max();
    for (const Board &move : possibleMoves) {
        int v = minimax(move, MaxDepth, a, b, true);
        if (v > a) {
            a = v;
            newBoard = move;
        }
    }
    return;
}
