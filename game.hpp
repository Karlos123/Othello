#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <QByteArray>
#include <QString>

#include "board.hpp"
#include "types.hpp"
#include "game-logic.hpp"
#include "history.hpp"


class Game{
    TColor playerColor{BLACK};  /**< Barva kamenu na tahu */
    TPlayer playerBlack{HUMAN}, playerWhite; /**< Logika hracu cerny a bily */
    int blackScore{2}, whiteScore{2};
    TAI AIType; /**< Typ inteligence, ktera vyhodnocuje dalsi tah */
  public:
    Board board;
    GameLogic gameLogic;
    History history;
    void nextTurn(void); /* Prepne na dasliho hrace */
    bool isEnd(void); /* Zjisti jestli nastal konec hry */
    bool isPvEGame() {return(playerWhite == AI);}; /* Zjisti jestli se hraje PC v clovek */
    bool execTurnHuman(int, int); /* Provedeni tahu cloveka */
    void execTurnAI(); /* Provedeni tahu pocitace */
    bool onTurnAI(); /* Vrati typ inteligence na tahu - HUMAN | AI */
    TAI  getAIType(void){return AIType;} /* Vraci verzi umele inteligence */
    TColor onTurnColor()const{ return(playerColor); }; /* Zjisti jaky hrac je tahu */
    int saveGame(QString fileName); /* Ulozi rozehranou hru */
    QByteArray loadGame(QString fileName); /* Nacte hru ze souboru */
    /* Nastavi hru */
    void setState(const TState &state) {board = state.board; playerColor = state.playerColor; blackScore = state.blackScore; whiteScore = state.whiteScore;};
    int getScore(TColor player){return player == BLACK ? blackScore : whiteScore;}; /* Vrati skore hrace */
    Game(int X, TPlayer A, TPlayer B, TAI AI) : board(X), history(board) {playerBlack = A; playerWhite = B; AIType = AI;};
};

#endif // GAME_HPP
