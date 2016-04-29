#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "board.hpp"
#include "types.hpp"
#include "game-logic.hpp"
#include "history.hpp"


class Game{
    TColor playerColor{BLACK};  // Hrac na tahu
    TPlayer playerBlack, playerWhite;
    int blackScore{2}, whiteScore{2};
    TAI AIType;
  public:
    Board board;
    GameLogic gameLogic;
    History history;
    void nextTurn(void);
    bool isEnd(void);
    bool execTurnHuman(int, int);
    void execTurnAI();
    TPlayer onTurnAI();
    TAI  getAIType(void){return AIType;}
    TColor onTurnColor()const{ return(playerColor); };
    int getScore(TColor player){return player == BLACK ? blackScore : whiteScore;};
    Game(int X, TPlayer A, TPlayer B, TAI AI) : board(X), history(board) {playerBlack = A; playerWhite = B; AIType = AI;};
};

#endif // GAME_HPP
