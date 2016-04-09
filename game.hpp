#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include "board.hpp"

enum PLAYER{ PBlack, PWhite};

class Game{
    int numPlayers{1};  // Pocet hracu
    int turn{PBlack};  // Hrac na tahu
    bool end{false};  // Identifikator konce hry
    bool stoneOK{false}; // Kamen spravne umisten
  public:
    void setNumPlayers(int N){  numPlayers = N; };
    void nextTurn();
    bool isEnd(){ return (end); };
    bool isPlaced() { return stoneOK;  };
    bool placeStone(int, int, Board&, PLAYER);
    PLAYER onTurn(){  return(turn == PBlack ?  PBlack : PWhite); };
    std::string printPlayer(){ return(turn == PWhite ? "white" : "black"); }; // CLI
};

#endif // GAME_HPP
