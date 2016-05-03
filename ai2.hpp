#ifndef AI2_HPP
#define AI2_HPP

#include "board.hpp"


/**
 * Nalezeni dalsiho tahu pomoci min-max doplneneho o alpha-betu
 * @param board       Aktualni stav herni desky
 * @param newBoard    Nove rozlozeni herni desky
 * @param playerColor Barva hrace na tahu
 */
void ai2NextState(const Board& board,  Board& newBoard,  TColor playerColor);


#endif // AI2_HPP
