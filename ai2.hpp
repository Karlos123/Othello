#ifndef AI2_HPP
#define AI2_HPP

#include "board.hpp"


/**
 * Dalsi tah je nalezen nahodne 
 * @param board       Aktualni stav herni desky
 * @param newBoard    Nove rozlozeni herni desky
 * @param playerColor Barva hrace na tahu
 */
void ai2NextState(Board& board,  TColor playerColor);


#endif // AI2_HPP
