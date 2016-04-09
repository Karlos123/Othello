//#include <QCoreApplication>
//#include <QTextStream>
#include <iostream>
#include <iomanip>
#include <limits>
#include <ctype.h>
#include <string>
#include "board.hpp"
#include "game.hpp"
#include "history.hpp"



/* Tisk chybove hlasky */
void printErr(const std::string msg){
  std::cerr << msg << std::endl;
}


/* MAIN */
int main()
{
  using namespace std;
  int X, Y;
  char Xc;
  Board board;
  Game game;
  History history;
  ios::sync_with_stdio(false);

  // Zacatek hry
  cout << endl  << "*** Welcome to the game Othello ***" << endl;
  cout << "Description:" << endl << "- 8x8" << endl << "- player vs. computer" << endl << "- only return in history" << endl;
  cout << "Game control: " << " " << endl;
  cout << setw(2) << " "<< endl;

  // Priprava
  board.construct(8);
  game.setNumPlayers(1);
  history.storeState(board);

  // Hra - Zacatek
  cout << "Let's begin..." << endl << endl;
  while(!game.isEnd()){
    board.print();
    // Umisteni kamene na desku
    while(!game.isPlaced()){
      cout << "Place stone or type command (" + game.printPlayer() << "): " << flush;
      // Nacteni X nebo prikazu
      for(cin>> Xc; isspace(Xc) && Xc != '\n'; cin >> Xc);
      cin.putback(Xc);
      cin >> Xc;
      switch (Xc) {
          case 'q':
          exit(0);
        case 'p':
          board = history.prevState(board);
          board.print();
          break;
        case '\n':
          continue;
        default:
          X = (unsigned char)Xc - 96;
      }
      // Nacteni Y
      cin >> Y;
      if(!cin){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printErr("Invalid stone position or command");
        continue;
      }
      // Polozeni kamene
      if(!game.placeStone(X, Y, board, game.onTurn())){
         printErr("Invalid stone position");
         continue;
      }
    } // END WHILE !game.isPlaced()

    history.storeState(board);     // Ulozeni aktualniho stau desky
    game.nextTurn(); // konec tahu
  }

  return 1;

}
