#ifndef AI1_HPP
#define AI1_HPP

#include "board.hpp"

/**
 * @breif  Prohleda od souradnice X, Y ve smeru dirX, dirY. Funkce vraci pozice novych
 * moznych kamenu pro hrace playerColor a vrati vector souradnic novych kamenu
*/
void ai1NextState(const Board& oldBoard, Board& newBoard, const TColor playerColor);

#endif // AI1_HPP
