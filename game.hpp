#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "board.hpp"
#include "types.hpp"
#include "game-logic.hpp"
#include "history.hpp"


class Game{
    TColor playerColor{BLACK};  // Hrac na tahu
    TAI player1, player2;
  public:
    Board board;
    GameLogic gameLogic;
    History history;
    void nextTurn();
    bool isEnd();
    bool execTurnHuman(int, int);
    void execTurnAI();
    TAI onTurnAI();
    TColor onTurnColor()const{ return(playerColor); };
    Game(int X, TAI A, TAI B) : board(X) {player1 = A; player2 = B;};
};

#endif // GAME_HPP
