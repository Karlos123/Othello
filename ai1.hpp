#ifndef AI1_HPP
#define AI1_HPP

#include "board.hpp"

/**
 * @brief  Prohleda od souradnice X, Y ve smeru dirX, dirY. Funkce vraci pozice novych
 * @param board Aktualni herni deska
 * @param playerColor Barva hrace pro ktereho se bude tah vyhodnocovat
 * moznych kamenu pro hrace playerColor a vrati vector souradnic novych kamenu
*/
void ai1NextState(Board& board, const TColor playerColor);

#endif // AI1_HPP
