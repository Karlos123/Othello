#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

#include "board.hpp"
#include "types.hpp"


class GameLogic{
    int X, Y; /**< Souradnice vstupniho kamene */
    TColor playerColor; /**< Barva kamenu hrace na tahu na tahu */
    TCordVec getNewStones(const Board& oldBoard, const int X, const int Y, const int dirX, const int dirY);
  public:
    void init(int a, int b, TColor c); /* Inicializace logiky - nastavi souradnice X, Y a barvy kamenu hrace na tahu */
    void checkPos(const Board&) const;  /* Kontroluje jestli je policko v rozsahu a je volne. Jinak vyhodi vyjimku "out_of_range" */
    void nextState(Board& board);  /* Vygeneruje dalsi stav hry za pomoci X, Y a playerColor */
    bool canTurn(Board&); /* Zjisti jestli muze aktulane nastaveny hrac tahnout kamenem */
    void countScore(const Board& board, int& blackScore,int& whiteScore); /* Prepocita aktualni skore */
    void markBoard(Board&); /* Oznaci mozne tahy na desku */
    void cleanBoard(Board&); /* Odstrani znacky o moznych tazich z desky */
};

#endif // GAMELOGIC_HPP
