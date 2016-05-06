#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <QByteArray>
#include <QString>
#include <ctime>

#include "board.hpp"
#include "types.hpp"
#include "game-logic.hpp"
#include "history.hpp"

/**
 * @class Game
 *
 * @brief Stara se o hru jako takovou.
 *
 * Trida uchovava  veskere auktualni informace o hre: barvu kamenu hrace na tahu, skore hracu, typ hracu (clovek, pocitac) a v pripade hry proti pocitaci i
 * nastaveni obtiznosti hry (AIType). Trida se predevsim stara o chod hry, vestinu logickych operaci a ukonu prenechava herni logicice se kterou je uzce
 * provazana.
 */
class Game{
    TColor playerColor{BLACK};  /**< Barva kamenu na tahu */
    TPlayer playerBlack{HUMAN}; /**< Logika hrace hrajiciho za cerne kameny */
    TPlayer playerWhite; /**< Logika hrace hrajiciho za bie  */
    int blackScore{2}; /**< Skore hrace hrajiciho za cerne kameny */
    int whiteScore{2}; /**< Skore hrace hrajiciho za bile kameny */
    TAI AIType; /**< Typ inteligence, ktera vyhodnocuje dalsi tah */
  public:
    Board board;
    GameLogic gameLogic;
    History history;

    void nextTurn(void); /* Prepne na dasliho hrace */

    bool isEnd(void); /* Zjisti jestli nastal konec hry */

    /**
     * @brief Vyhodnoti jestli se jedna o hru hrac vs pocitac nebo hrac vs hrac
     * @return True Kdyz se jedna o hru hrace proti pocitaci. JIank false
     */
    bool isPvEGame() {return(playerWhite == AI);};

    bool execTurnHuman(int, int); /* Provedeni tahu cloveka */

    void execTurnAI(); /* Provedeni tahu pocitace */

    bool onTurnAI(); /* Vrati typ inteligence na tahu - HUMAN | AI */

    TAI  getAIType(void){return AIType;} /* Vraci verzi umele inteligence */

    /**
     * @brief Nastaveni logiky hrace bily.
     * @param p Logika hrace bily (HUMAN | AI)
     */
    void setOpponentType(TPlayer p){playerWhite = p;};

    /**
     * @brief Zjisti barvu kamenu hracee na tahu
     * @return Barva kamenu hrace na tahu
     */
    TColor onTurnColor()const{ return(playerColor); };

    int saveGame(QString fileName); /* Ulozi rozehranou hru */

    QByteArray loadGame(QString fileName); /* Nacte hru ze souboru */

    /**
     * @brief Nastaveni hernich promennych herni deska, hrac na tahu, skore hrace za cerne a bile kameny ze stavu ulozenoh v historii hry.
     * @param state Ulozeny stav z historie
     */
    void setState(const TState &state) {board = state.board; playerColor = state.playerColor; blackScore = state.blackScore; whiteScore = state.whiteScore;};

    /**
     * @brief Vrati skore hrace specifikovaneho v parametru player
     * @param  player Hrac u ktereho pozadujeme skro
     * @return        Skore hrace
     */
    int getScore(TColor player){return player == BLACK ? blackScore : whiteScore;}; /* Vrati skore hrace */

    /**
     * @brief Konstruktor hry, kteruy inicializuje herni desku, logiku hry, historii. Dale nastavuje tridni promenne.
     * @param X Velikost herni desky
     * @param A Logika hrace hrajiciho za cerne kameny
     * @param B Logika hrace hrajiciho za bile kameny
     * @param AI Typ inteligence pocitace (obtiznsost)
     */
    Game(int X, TPlayer A, TPlayer B, TAI AI) : board(X), history(board) {playerBlack = A; playerWhite = B; AIType = AI; std::srand(time(0));};
};

#endif // GAME_HPP
