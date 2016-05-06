#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

#include "board.hpp"
#include "types.hpp"

/**
 * @Class GameLogic
 *
 * @breif Herni logika hry starajici se o logicke operace nad hernim planem.
 *
 * Herni logika poskytuje funkce tykajici se posunu kamenu na herni desce, generovani dalsiho rozlozeni kamenu pro hru hrac vs hrac a hrac vs pocitac.
 * Pred provadenim jakyvkoliv zmen je nutna inicilizace vychozich souradnic X, Y a barvy hrace pro ktereho se budou jednotlive funkce provadet.
 * Nektere funkce, ktere poskytuje trida GameLogic nevyzaduji inicializaci vsech promennych (napr.: markBoard).
 */
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
