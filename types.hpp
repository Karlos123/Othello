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

typedef enum {HUMAN, AI} TAI;



#endif // TYPES_HPP
