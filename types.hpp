#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>

/**
 * @enum TColor
 * @brief Barva kamene na desce. Barva hrace.
 */
typedef enum { NONE, BLACK, WHITE, MARKSTONE} TColor;

/**
 * TCord
 * @brief Souradnice X,Y na hernim planu
 */
typedef std::pair<int,int> TCord;

/**
 * TCordVec
 * @brief Vektor souradnic X,Y
 */
typedef std::vector<TCord> TCordVec;

/**
 * TStones
 * @brief Pole kamenu tvorene vektorem vektroru typu TColor
 * @note Priklad pristupu k jednotlivemu kameni ve vektoru stones typu TStones: stones[X][Y]
 */
typedef std::vector<std::vector<TColor>> TStones;

/**
 * @enum TPlayer
 * @brief Logika hrace
 */
typedef enum {HUMAN, AI} TPlayer;

/**
 * @enum TAI
 * @brief Typ logiky umele inteligence (obtiznost).
 */
typedef enum {AI_EASY, AI_DIFFICULT} TAI;


#endif // TYPES_HPP
