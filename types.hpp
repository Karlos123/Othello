#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>

// Typ kamene
typedef enum { NONE, BLACK, WHITE} TColor;

// Souradnice na hernim planu (X,Y)
typedef std::pair<int,int> TCord;

// Vektor souradnic
typedef std::vector<TCord> TCordVec;

// Vektor kamenu
typedef std::vector<std::vector<TColor>> TStones;

// Typ hrace
typedef enum {HUMAN, AI} TPlayer;

// Typ umele inteligence
typedef enum {AI_AB, AI_SIMPLE} TAI;


#endif // TYPES_HPP
