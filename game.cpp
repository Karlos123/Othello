#include "game.hpp"


/* Prechod na dalsi tah */
void Game::nextTurn(){
  turn = turn == PBlack ? PWhite : PBlack;
  stoneOK = false;
}


/* Umisteni kamene na desku */
bool Game::placeStone(int X, int Y, Board& board, PLAYER player){
  if(X < 0 || Y < 0 || X > board.isSize() || X > board.isSize() || !board.isEmpty(X -1, Y-1) )
    return false;
  else{
    board.putStone(X-1, Y-1, player == PWhite ?  CWhite : CBlack);
    stoneOK = true;
  }
  return true;
}
