#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "board.hpp"
#include "types.hpp"
#include "game-logic.hpp"
#include "history.hpp"


class Game{
    TColor playerColor{BLACK};  // Hrac na tahu
    TPlayer player1, player2;
  public:
    Board board;
    GameLogic gameLogic;
    History history;
    void nextTurn();
    bool isEnd();
    bool execTurn(int, int, Board&);
    TColor onTurn()const{ return(playerColor); };
    std::string getPlayerString()const{ return(playerColor == WHITE ? "white" : "black"); }; // CLI
    Game(int X, TPlayer A, TPlayer B) : board(X) {player1 = A; player2 = B;};
};

#endif // GAME_HPP
