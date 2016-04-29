//#include <QCoreApplication>
//#include <QTextStream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <ctype.h>
#include <string>
#include <regex>
#include "game.hpp"



/* Vytiskne popis hry */
void printDescription(){
  std::cout << "  * Board 8x8" << std::endl
            << "  * Player vs. computer" << std::endl
            << "  * Only return in history" << std::endl;
}

/* Vytiskne ovladani hry */
void printControl(){
  std::cout << "  * Type coordinates to to place the stone" << std::endl
            << "  * Exceptable coordinate patterns:" << std::endl
            << "    - 6a (digit letter)"  << std::endl
            << "    - a6 (letter digit)" << std::endl
            << "  * All whitespaces (except newline) are ignored" << std::endl
            << "  * To quit the qame type \"q\"" << std::endl;
}

/* Nacte X, Y nebo provede prikaz z prikazove radky */
bool loadCin(int& X, int& Y){
  std::string cmdLine;
  std::smatch matches;

  std::getline(std::cin,cmdLine);
  // Nacteni X, Y
  if(std::regex_match(cmdLine, matches, std::regex("(\\s*([a-z])\\s*(\\d)+\\s*|\\s*(\\d)+\\s*([a-z])\\s*)"))){
    if(static_cast<std::string>(matches[3]).empty()){ // Druha cast reg. vyrazu uspela
      X = std::strtol(static_cast<std::string>(matches[4]).c_str(), NULL, 0);
      Y = (unsigned char)static_cast<std::string>(matches[5])[0] - 96; // prevod pismena na cislo
    }
    else{ // Prvni cast regulrniho vyrazu uspela
      X = std::strtol(static_cast<std::string>(matches[3]).c_str(), NULL, 10);
      Y = (unsigned char)static_cast<std::string>(matches[2])[0] - 96;
    }
  }
  // nacteni X, Y selhalo -> nacteni prikazu
  else if(std::regex_match(cmdLine, matches, std::regex("(\\s*[q|n]\\s*)"))){
    switch(static_cast<std::string>(matches[1])[0]){
      case 'q': exit(0);
    }
  }
  // Nacteni prikazy | X, Y bylo neuspesne
  else{
    return false;
  }
  return true;
}

/* MAIN */
int main()
{
  int X, Y;

  // Zacatek hry
  std::cout << std::endl  << "*** Welcome to the game Othello ***" << std::endl << std::endl;
  std::cout << "Description: " << std::endl; printDescription(); std::cout << std::endl;
  std::cout << "Game control: " << std::endl; printControl(); std::cout << std::endl;
  std::cout << std::setw(2) << " "<< std::endl;

  // Priprava
  Game game{8, HUMAN, AI, AI_AB};

  // Hra - Zacatek
  while(!game.isEnd()){
    game.board.print();
    // Nacteni souradnic X,Y a provedeni tahu
    std::cout << "Score B: " << game.getScore(BLACK) << " W: " << game.getScore(WHITE) << std::endl << std::endl;
    std::cout << "Playing (" << (game.onTurnColor() == WHITE ? "white" : "black")
              << "-" << (game.onTurnAI() == AI ? "AI" : "human")  << "): "
              << (game.onTurnAI() == AI ? "\n" : "") <<  std::flush;
    // Na tahu umela inteligence
    if(game.onTurnAI() == AI){
      game.execTurnAI();
      continue;
    }

    while(!loadCin(X,Y) || !game.execTurnHuman(X-1, Y-1))
      std::cout << "Playing (" << (game.onTurnColor() == WHITE ? "white" : "black")
              << "-" << (game.onTurnAI() == AI ? "AI" : "human")  << "): "
              << (game.onTurnAI() == AI ? "\n" : "") <<  std::flush;
  }
  std::cout << "END" << std::endl;
   return 1;
}
