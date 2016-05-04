#include "ai2.hpp"
#include "board.hpp"

#include <algorithm>
#include <limits>
#include <cstdlib>
#include <iostream>


const int MaxDepth = 4; /**< Maximalni hloubka prohledavani */

// Globalni promenne slouzici k optimalizaci
int size; /**< velikost herniho planu - optimalizace */
TColor curPlayer; /**< Aktualni hrac na tahu */

/**
 * @breif  Prohleda od souradnice X, Y ve smeru dirX, dirY. Funkce vraci pozice novych
 * moznych kamenu pro hrace playerColor a vrati vector souradnic novych kamenu
 * @param  oldBoard Vychozi deska
 * @param  dirX     Smer prohledavani v ose x (radek)
 * @param  dirY     Smer prohledavani v ose y (sloupec)
 * @param  playerColor Barva hrace na tahu
 * @return          Vektor souradnic X,Y novych kamenu aktulaniho hrace
 */
TCordVec findNewVec(const Board& oldBoard, const int X, const int Y, const int dirX, const int dirY, TColor playerColor){
  // Policko neni prazdne
  if(oldBoard.getStone(X,Y) != NONE)
    return {};

  int i = X+dirX; /**< Posun na dalsi kamen v ose x */
  int j = Y+dirY;  /**< Posun na dalsi kamen v ose y */
  TCordVec newStones{}; /**< Vector novych kamenu hrace ktery je na tahu */
  TColor rivalColor = playerColor == BLACK ? WHITE : BLACK; /**< Souperova barva */

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

bool findAnyMove(const Board& board, const int X, const int Y, const int dirX, const int dirY, TColor player){
  if(board.getStone(X,Y) != NONE)
    return false;
  int i = X+dirX;
  int j = Y+dirY;
  TColor rival = player == BLACK ? WHITE : BLACK; /**< Soupeova barva */

  // Prohledavej herni pole
  while(board.inRange(i,j)){
    // Kamen na pozici neni souperuv
    if(board.getStone(i,j) != rival){
      // Kamen je hrace na tahu = OK
      if(board.getStone(i,j) == player)
        return true;
      else
        return false;
    }
    i+=dirX;
    j+=dirY;
  }
  return false; // Vyhledavani skoncilo mimo herni plan
}

/**
 * Zjisti jestli nastal konec
 * @param  board [description]
 * @return       [description]
 */
bool isEnd(const Board& board){
  // Prohledavani celeho herni desky
  for (int X = 0; X < size; X++) {
    for (int Y = 0; Y < size; Y++) {
      // Na policku je kamen
      if(board.getStone(X, Y) != NONE)
        continue;
      // Prohledavani okoli kamene na pozici i,j
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if(i%3-1 == 0 && j%3-1 == 0) // Pozice kamene + smery prohledavani je 0 a 0
            continue;
            if(findAnyMove(board, X, Y, i%3-1, j%3-1, WHITE))
              return false;
            if(findAnyMove(board, X, Y, i%3-1, j%3-1, BLACK))
              return false;
          }
        }
      }
    }
    return true;
  }


/**
 * @breif Nalezeni vsech moznych tahu
 * @param possibleMoves Vektor moznych tahu
 * @param oldBoard      Vychozi deska
 * @param playerColor   Barva hrace pro ktereho jsou vyhledavany vektory tahu
 */
void findPossibleMoves(std::vector<Board>& possibleMoves, const Board& oldBoard, const TColor playerColor){
  TCordVec newStones{}; /**<  Vektor novych kamenu */
  TCordVec tmpv{}; /**< Aktualni pomocny vektor novych kamenu */
  Board possibleMove{size}; /**< Dalsi mozny tah */

  // Prohledavani celeho herni desky
  for (int X = 0; X < size; X++) {
    for (int Y = 0; Y < size; Y++) {
      // Prohledavani okoli kamene
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          if(i%3-1 == 0 && j%3-1 == 0)
            continue;
          tmpv = findNewVec(oldBoard, X, Y, i%3-1, j%3-1, playerColor); // Nalezeni noveho vektoru kamenu (souradnic)
          if(!tmpv.empty())
            newStones.insert(newStones.end(), tmpv.begin(), tmpv.end()); // Sjednoceni vektoru
        }
      }
      // Nalezen novy vektor kamenu
      if(!newStones.empty()){
        possibleMove = oldBoard; // Inicializace vychozi deskou
        newStones.push_back(std::make_pair(X,Y)); // Pridani pokladaneho kamene do vektoru
        for(const TCord& i : newStones) // Umisteni kamenu na desku
          possibleMove.putStone(i.first, i.second, playerColor);
        possibleMoves.push_back(possibleMove); // Ulozeni mozneho rozlozeni
      }
      newStones = {};
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
    if(blackScore == 0)
      return std::numeric_limits<int>::max();
    return whiteScore - blackScore;
}


bool canTurn(const Board& board, TColor player){
  TCordVec vecCords, tmp;
  // Prohledavani celeho herni desky
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
          tmp = findNewVec(board, X, Y, i%3-1, j%3-1, player );
          if(!tmp.empty())
            return true;
        }
      }
    }
  }
  return false;
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
    if (depth == 0 || (!canTurn(board, WHITE) &&  !canTurn(board, BLACK)))
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
